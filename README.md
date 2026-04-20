# Multiplay By Rocket Science Unreal Engine SDK 

## Prerequisites

* Unreal Engine: minimum version 4.26.2. Verified against UE 4.26.2 through UE 5.5.x — see [CHANGELOG.md](CHANGELOG.md) for explicit compatibility fixes (UE 5.4.0 in v1.1.3, UE 5.5.0 in v1.1.5).
* Operating System: Windows 10 / 11 or Ubuntu
* IDE: Visual Studio 2019 or 2022 (recommended for UE 5.x)
* Linux Toolchain: `v20_clang-13.0.1-centos7` (required for cross-compiling UE 5.1 targets)

Required Visual Studio modules are listed in the [Appendix](#appendix).

## Install the plugin

Install the plugin by adding it to your project's `Plugins/` directory.

The SDK is distributed as source rather than as a prebuilt binary. The reasons
are:

1. **Unreal's ABI changes between engine versions.** A binary plugin built
   against one UE version will not reliably link against another, even a
   point release apart. The plugin must be compiled against the exact engine
   version your project uses, so the source has to ship to do that.
2. **Unreal Build Tool recompiles all plugins when you package a dedicated
   server.** UBT drives the dedicated-server build by walking every enabled
   plugin's `*.Build.cs`, generating module targets, and compiling them from
   source into the server binary. There is no stage in that pipeline where a
   prebuilt plugin binary can be slotted in.
3. **Teams need to modify plugins.** Adding logging, changing default
   behaviour, patching a bug against a specific engine version, or wiring the
   plugin into other systems all require editing source. Source distribution
   makes that the path of least resistance, rather than a fork-and-rebuild
   exercise.

### Plugin folder naming

Place the plugin source under `Plugins/RocketScienceMultiplaySDK/` at the
root of your Unreal project (the directory containing your `.uproject`).

The folder name must match the `.uplugin` file basename. Unreal resolves
plugins by matching the folder name to `<FolderName>.uplugin`, so this
repository's `RocketScienceMultiplaySDK.uplugin` must live inside a folder
called `RocketScienceMultiplaySDK`. Using any other folder name causes the
plugin to be silently ignored — no error, just no plugin.

```
YourProject/
├── YourProject.uproject
└── Plugins/
    └── RocketScienceMultiplaySDK/
        ├── RocketScienceMultiplaySDK.uplugin
        └── Source/
```

### Enable the plugin

1. Open your project in the Unreal Editor.
2. Go to **Edit > Plugins**.
3. Search for **Rocket Science Multiplay SDK for Unreal Engine**.
4. Enable it and restart the editor when prompted.

Alternatively, edit your `.uproject` file directly:

```json
"Plugins": [
    {
        "Name": "RocketScienceMultiplaySDK",
        "Enabled": true
    }
]
```

### Modules

The plugin ships two modules:

| Module | Type | Platforms |
| ----------- | ----------- | ----------- |
| `RocketScienceMultiplaySDK` | ServerOnly | Linux, Win64 |
| `RocketScienceQos` | ClientOnly | Linux, Mac, Win64, PS4, PS5, Switch, Switch2, XboxOneGDK, XSX |

Add the modules you need to your `*.Build.cs` `PublicDependencyModuleNames`:

```csharp
PublicDependencyModuleNames.AddRange(new string[] {
    "RocketScienceMultiplaySDK",  // server builds
    "RocketScienceQos",           // client builds
});
```

## Using the subsystems

The SDK exposes its functionality through Unreal `UGameInstanceSubsystem` classes.
Retrieve any subsystem via `GetGameInstance()->GetSubsystem<T>()`.

### Game Server subsystem

`URSMultiplayGameServerSubsystem` is the primary entry point for server code. It
communicates with the Multiplay SDK daemon running locally on the game server
machine, reporting server readiness and receiving allocation lifecycle events.

Use this subsystem to:

* Mark the server ready or unready for players, which controls whether Multiplay
  Hosting's allocation system can place a game session on it.
* Subscribe to allocation and deallocation events fired by the matchmaker or
  the Multiplay Hosting API.
* Retrieve the allocation payload — an opaque string (up to 30 KB) that a
  matchmaker attaches to an allocation request to configure the session on a
  per-match basis. The platform treats the payload as arbitrary UTF-8 text and
  does not enforce a format, but **JSON is the recommended convention**:
    * JSON comfortably fits inside the 30 KB size cap for typical match
      configuration (mode, map rotation, player slots, mod flags, etc.).
  The Unreal SDK delivers the payload to you as an `FString` — parse it with
  `FJsonSerializer::Deserialize` if you adopt the JSON convention, or handle
  your chosen format directly.

| Member | Description |
| ----------- | ----------- |
| `ReadyServerForPlayers(OnSuccess, OnFailure)` | Marks the server ready. Call this once the server has finished loading assets, connected to external services, and is prepared to accept players. Readiness is an optional attribute on the build configuration — the allocation system only gates allocations on readiness when that flag is set on the build configuration in the Multiplay Dashboard. If readiness is not enabled on the build configuration, this call has no effect on allocation eligibility. |
| `UnreadyServer(OnSuccess, OnFailure)` | Marks the server unready. Call this when shutting down, entering maintenance, or when the server is no longer safe to allocate to. Same build-configuration conditionality as `ReadyServerForPlayers` — the unready state only affects allocation eligibility when readiness is enabled on the build configuration. |
| `SubscribeToServerEvents()` | Opens a connection to the SDK daemon and starts listening for allocation messages. Call this during server startup before `ReadyServerForPlayers`. |
| `UnsubscribeToServerEvents()` | Closes the connection and stops listening for events. |
| `GetPayloadAllocation(OnSuccess, OnFailure)` | Retrieves the payload attached to the current allocation. The payload is only available until the allocation ends or its TTL elapses. |
| `OnAllocate` | Multicast delegate fired when the server is allocated. The delegate receives an `FRSMultiplayAllocation` struct with three fields: `EventId` (the unique ID of the allocation event itself), `ServerId` (the server the allocation targets), and `AllocationId` (the allocation UUID used for deallocation and payload lookups). |
| `OnDeallocate` | Multicast delegate fired when the server is deallocated. The delegate receives an `FRSMultiplayDeallocation` struct containing the matching event and allocation identifiers. |

Example:

```cpp
auto* GameServer = GetGameInstance()->GetSubsystem<URSMultiplayGameServerSubsystem>();
GameServer->OnAllocate.AddDynamic(this, &AMyServer::HandleAllocated);
GameServer->OnDeallocate.AddDynamic(this, &AMyServer::HandleDeallocated);
GameServer->SubscribeToServerEvents();

FRSReadyServerSuccessDelegate OnSuccess;
OnSuccess.BindDynamic(this, &AMyServer::HandleReadySuccess);
FRSReadyServerFailureDelegate OnFailure;
OnFailure.BindDynamic(this, &AMyServer::HandleReadyFailure);
GameServer->ReadyServerForPlayers(OnSuccess, OnFailure);
```

### Server Config subsystem

`URSMultiplayServerConfigSubsystem` exposes the contents of the `server.json`
file that Multiplay Hosting generates and maintains for every game server
instance. It is the Unreal equivalent of reading the file directly, without the
need to poll or watch the file system.

The config is populated from the build configuration's configuration variables
plus built-in variables such as the allocation ID, ports, fleet ID, and region.
Values may change during a server's lifetime — most notably `AllocationId`,
which is set when the server is allocated and cleared when it is deallocated.

| Member | Description |
| ----------- | ----------- |
| `GetServerConfig()` | Returns the current `FRSMultiplayServerConfig` struct. |

`FRSMultiplayServerConfig` fields:

| Field | Type | Description |
| ----------- | ----------- | ----------- |
| `ServerId` | `int64` | Unique identifier of this server instance. |
| `AllocationId` | `FString` | Current allocation UUID. Empty while unallocated. |
| `Ip` | `FString` | IP address the server is bound to. |
| `Port` | `int32` | Game port for player traffic. |
| `QueryPort` | `int32` | Port the server query protocol (SQP) listens on. |
| `ServerLogDirectory` | `FString` | Directory Multiplay Hosting expects server logs to be written to, so they are exposed via the Dashboard. |

### Extending server.json with custom configuration variables

The fields above are the built-in subset that `FRSMultiplayServerConfig`
surfaces. Multiplay Hosting also lets you define custom configuration variables
on a build configuration in the Multiplay Dashboard — things like a difficulty
modifier, game mode, or map selection. These variables are written into the
same `server.json` file alongside the built-ins at server start and whenever
they change.

Custom variables are not exposed through `FRSMultiplayServerConfig` directly.
To read them, open `server.json` from `$HOME/server.json` (Linux) or
`$HOMEPATH/server.json` (Windows) and parse the additional fields yourself. The
built-in fields on the struct will continue to reflect the values injected by
Multiplay Hosting, so the two approaches can be mixed freely.

### Configuration variable tokens (`$$...$$`)

Multiplay Hosting uses a `$$name$$` token syntax for substitution variables.
Tokens like `$$port$$`, `$$query_port$$`, `$$serverid$$`, `$$log_dir$$`, and
`$$timestamp$$` are replaced with per-server values at server start. The same
token resolves to the same value wherever it appears — in a launch parameter,
in a custom configuration variable, or in the built-in fields of `server.json`
— so a launch argument `-port=$$port$$` and a `server.json` field `"port"`
will always carry the matching integer at runtime.

For the full list of available variables and their types, see the Multiplay
customer documentation: [Server variables][server-variables].

[server-variables]: https://open-2v.gitbook.com/multiplay-customer-docs/reference/server-variables

Example:

```cpp
auto* Config = GetGameInstance()->GetSubsystem<URSMultiplayServerConfigSubsystem>();
const FRSMultiplayServerConfig& Cfg = Config->GetServerConfig();

UE_LOG(LogMyGame, Log, TEXT("Serving on %s:%d (server %lld)"),
    *Cfg.Ip, Cfg.Port, Cfg.ServerId);
```

### Server Query Handler subsystem

`URSMultiplayServerQueryHandlerSubsystem` implements the Server Query Protocol
(SQP) that Multiplay Hosting uses to detect unresponsive servers, drive live
analytics (concurrently connected users, crashes, server events), and surface
server state on the Dashboard. When `Connect()` is called the subsystem binds
a UDP socket using the `queryPort` value read from `server.json` (via
`URSMultiplayServerConfigSubsystem`) and responds to incoming SQP requests
automatically.

Your game code is responsible for keeping the reported state valid and fresh
— principally `CurrentPlayers`, but also `MaxPlayers`, `ServerName`,
`GameType`, `BuildId`, `Map`, and `Port`. Multiplay Hosting uses SQP as the
primary **health check** for the game server process: the hosting platform
polls the SQP endpoint on an interval, and a run of invalid or missing
responses is treated as "server crashed" — regardless of whether the game
process is still running. Examples of invalid responses include:

* Failing the SQP challenge / response handshake.
* Returning a `CurrentPlayers` value higher than `MaxPlayers`.
* Returning malformed packets, truncated data, or responses that exceed the
  protocol's size bounds.
* Not responding at all for a sustained period.

Once Multiplay Hosting concludes the server has crashed, the platform takes
the standard crash actions (deallocation, restart or recycle, incident
counters, etc.), which can end the match mid-session. The same SQP stream
also feeds Multiplay Hosting's monitoring and analytics for the hosting
infrastructure as a whole — concurrently connected users, crash rates, map
distribution, and so on are all derived from the values you report here, so
accuracy matters.

| Member | Description |
| ----------- | ----------- |
| `Connect()` | Binds the UDP socket using `FRSMultiplayServerConfig::QueryPort` and starts listening for SQP requests. Returns `false` if the port could not be bound. The bind port is not configurable through the subsystem — see [Binding port and command-line overrides](#binding-port-and-command-line-overrides) below. |
| `Disconnect()` | Closes the UDP socket and stops responding to queries. |
| `IsConnected()` | Returns whether the subsystem is currently listening. |
| `GetCurrentPlayers()` / `SetCurrentPlayers(Value)` | Accessors for the current connected-player count (max 65535). |
| `IncrementCurrentPlayers()` / `DecrementCurrentPlayers()` | Atomic helpers for updating the player count when a player joins or leaves. |
| `GetMaxPlayers()` / `SetMaxPlayers(Value)` | Accessors for the maximum players the server supports (max 65535). |
| `GetServerName()` / `SetServerName(Value)` | Accessors for the server's display name (max 255 characters). |
| `GetGameType()` / `SetGameType(Value)` | Accessors for the game type the server is currently running (max 255 characters). |
| `GetBuildId()` / `SetBuildId(Value)` | Accessors for the build ID (max 255 characters, ASCII only). |
| `GetMap()` / `SetMap(Value)` | Accessors for the currently loaded map name (max 255 characters). |
| `GetPort()` / `SetPort(Value)` | Accessors for the **reported game port** that the subsystem advertises in its SQP `ServerInfo` response. This is the port game clients connect to — it is **not** the UDP bind port. Should be set from `FRSMultiplayServerConfig::Port`. |

Example:

```cpp
auto* Query = GetGameInstance()->GetSubsystem<URSMultiplayServerQueryHandlerSubsystem>();
auto* Config = GetGameInstance()->GetSubsystem<URSMultiplayServerConfigSubsystem>();

// Report the game port to SQP clients. The UDP bind port for SQP itself is
// read separately from FRSMultiplayServerConfig::QueryPort when Connect() runs.
Query->SetPort(Config->GetServerConfig().Port);
Query->SetMaxPlayers(32);
Query->SetServerName(TEXT("My Game Server"));
Query->SetGameType(TEXT("Deathmatch"));
Query->Connect();

// On player join:
Query->IncrementCurrentPlayers();
```

#### Binding port and command-line overrides

A [Steam A2S][a2s] query port is the UDP port that Valve's `A2S_INFO` /
`A2S_PLAYER` / `A2S_RULES` requests are answered on. SQP serves the same role
for Multiplay, and A2S is in fact supported as well — declare which protocol
your build uses in the build configuration's query type on the Multiplay
Dashboard, and Multiplay Hosting will poll it accordingly.

[a2s]: https://developer.valvesoftware.com/wiki/Server_queries

> **Warning — do not bind another query handler to
> `-queryport=$$query_port$$`.** If another subsystem in your project (for
> example Steam's `OnlineSubsystemSteam` A2S handler, or any custom query
> implementation) is already wired to the default
> `-queryport=$$query_port$$` command-line argument, it will land on the
> same UDP port this SDK tries to bind for SQP. `$$query_port$$` is
> resolved consistently everywhere Multiplay Hosting substitutes it, so the
> launch parameter and `server.json`'s `queryPort` field carry the **same
> integer** at runtime. Two subsystems racing for one UDP bind has two bad
> outcomes:
>
> * The bind itself fails for one of them, with the losing subsystem
>   silently dropping off. Which one loses depends on initialisation order.
> * The wrong protocol answers on the port Multiplay Hosting polls, so
>   responses do not match the query type declared on the build
>   configuration. Multiplay's health check treats that as an unresponsive
>   server, and the standard crash actions (deallocation, restart or
>   recycle, incident counters) follow — even though the process is alive
>   and a different protocol is answering on the same port.
>
> **Recommendation: do not run A2S (or any other query protocol) alongside
> this SDK.** Declare SQP as the query type on your build configuration and
> let this SDK handle it exclusively. If your project already relies on
> another query handler (Steam A2S or otherwise), choose one and disable
> the other before enabling this SDK. If nothing else in your project is
> bound to `-queryport=`, this concern does not apply.

##### Why this SDK ignores the command line

Unreal's default (`OnlineSubsystemSteam`) reads the query port from
`FParse::Value` or `Engine.ini`. This SDK deliberately does **not** follow that
pattern:

1. Multiplay Hosting is the source of truth for port assignment. It generates
   a `queryPort` per server instance, writes it to `server.json`, and expects
   the running process to bind that exact port so it can reach it for health
   and analytics polling.
2. Multiple server instances can share a single machine under Multiplay
   Hosting. Any static convention — such as `27015`, or a single CLI value per machine — would collide across
   game servers. `server.json`'s `queryPort:$$query_port$$` is per game server instance and assigned by
   the platform.
3. Source builds and dedicated-server repackages happen frequently. Hard-wiring
   the port discovery to the same file Multiplay already writes avoids a
   whole class of "forgot the flag" footguns.

For those reasons `server.json` is authoritative and the subsystem reads it
directly.

##### Remapping `-queryPort` into the SDK

If your team wants command-line override behaviour to align with Unreal's
`OnlineSubsystemSteam` convention — for example to run a locally-launched
server without a `server.json` file — the plugin ships as source, so the
recommended approach is to extend
`URSMultiplayServerQueryHandlerSubsystem::Connect()` with a CLI fallback:

```cpp
// In MultiplayServerQueryHandlerSubsystem.cpp, inside Connect():
int32 QueryPort = ServerConfig.QueryPort;

if (QueryPort == 0)
{
    // Fall back to -queryPort=NNNN to align with Unreal's OnlineSubsystemSteam
    // convention. FParse is case-insensitive so this also matches -QueryPort=
    // and -queryport=.
    FParse::Value(FCommandLine::Get(), TEXT("queryPort="), QueryPort);
}

if (QueryPort == 0)
{
    UE_LOG(LogRSMultiplayGameServerSDK, Error,
        TEXT("Query port not set in server.json and not provided on the command line"));
    return false;
}
```

Keep the `server.json` value as the primary source so the plugin remains
aligned with Multiplay Hosting's configuration model. The command-line path is
a fallback for local dev and test scenarios.

### Complete server bootstrap example

The snippets in the subsystem sections show each API in isolation. In a real
dedicated-server build you need them wired together in the right order. The
simplest way to do that is a custom `UGameInstance` subclass whose `Init()`
overrides the server startup sequence.

The engine auto-creates `UGameInstanceSubsystem`s during `UGameInstance`
construction, so there is no need to instantiate the Multiplay subsystems
yourself — retrieve them via `GetSubsystem<T>()` once the game instance is up.

Point your project's default game instance class at `UMyGameInstance` in
**Project Settings → Maps & Modes → Game Instance Class** (or via
`DefaultEngine.ini` under `[/Script/EngineSettings.GameMapsSettings]`).

```cpp
// MyGameInstance.h
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RSMultiplayAllocation.h"
#include "RSMultiplayDeallocation.h"
#include "RSMultiplayErrorResponse.h"
#include "MyGameInstance.generated.h"

UCLASS()
class MYGAME_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

private:
    UFUNCTION()
    void HandleAllocated(FRSMultiplayAllocation Allocation);

    UFUNCTION()
    void HandleDeallocated(FRSMultiplayDeallocation Deallocation);

    UFUNCTION()
    void HandleReadySuccess();

    UFUNCTION()
    void HandleReadyFailure(FRSMultiplayErrorResponse ErrorResponse);
};
```

```cpp
// MyGameInstance.cpp
#include "MyGameInstance.h"
#include "RSMultiplayGameServerSubsystem.h"
#include "RSMultiplayServerConfigSubsystem.h"
#include "RSMultiplayServerQueryHandlerSubsystem.h"

void UMyGameInstance::Init()
{
    Super::Init();

    // Guard: the Multiplay SDK is a dedicated-server concern only. Skip on
    // clients, listen servers, and PIE clients.
    if (!IsDedicatedServerInstance())
    {
        return;
    }

    auto* GameServer = GetSubsystem<URSMultiplayGameServerSubsystem>();
    auto* Config     = GetSubsystem<URSMultiplayServerConfigSubsystem>();
    auto* Query      = GetSubsystem<URSMultiplayServerQueryHandlerSubsystem>();

    // 1. Wire allocation lifecycle delegates before subscribing so we do not
    //    miss an event that fires between subscribe and delegate binding.
    GameServer->OnAllocate.AddDynamic(this, &UMyGameInstance::HandleAllocated);
    GameServer->OnDeallocate.AddDynamic(this, &UMyGameInstance::HandleDeallocated);

    // 2. Open the connection to the local SDK daemon.
    GameServer->SubscribeToServerEvents();

    // 3. Configure and start the SQP query handler. Uses server.json values
    //    from ServerConfig — no bind port is passed; it is read internally.
    Query->SetPort(Config->GetServerConfig().Port);   // reported game port
    Query->SetMaxPlayers(32);
    Query->SetServerName(TEXT("My Game Server"));
    Query->SetGameType(TEXT("Deathmatch"));
    Query->SetBuildId(TEXT("1.0.0"));
    Query->SetMap(TEXT("arena"));
    Query->Connect();

    // 4. Tell Multiplay Hosting the server is ready to accept allocations.
    //    Only meaningful when the build configuration has readiness enabled;
    //    otherwise this is a no-op on the allocation system.
    FRSReadyServerSuccessDelegate OnSuccess;
    OnSuccess.BindDynamic(this, &UMyGameInstance::HandleReadySuccess);
    FRSReadyServerFailureDelegate OnFailure;
    OnFailure.BindDynamic(this, &UMyGameInstance::HandleReadyFailure);
    GameServer->ReadyServerForPlayers(OnSuccess, OnFailure);
}

void UMyGameInstance::Shutdown()
{
    if (IsDedicatedServerInstance())
    {
        if (auto* Query = GetSubsystem<URSMultiplayServerQueryHandlerSubsystem>())
        {
            Query->Disconnect();
        }
        if (auto* GameServer = GetSubsystem<URSMultiplayGameServerSubsystem>())
        {
            GameServer->UnsubscribeToServerEvents();
        }
    }
    Super::Shutdown();
}

void UMyGameInstance::HandleAllocated(FRSMultiplayAllocation Allocation)
{
    UE_LOG(LogTemp, Log, TEXT("Allocated: %s (server %lld)"),
        *Allocation.AllocationId, Allocation.ServerId);
    // Load the match configuration, travel to the gameplay map, etc.
}

void UMyGameInstance::HandleDeallocated(FRSMultiplayDeallocation Deallocation)
{
    UE_LOG(LogTemp, Log, TEXT("Deallocated"));
    // Tear down match state, return to idle, or shut down the process.
}

void UMyGameInstance::HandleReadySuccess()
{
    UE_LOG(LogTemp, Log, TEXT("Server marked ready"));
}

void UMyGameInstance::HandleReadyFailure(FRSMultiplayErrorResponse ErrorResponse)
{
    UE_LOG(LogTemp, Error, TEXT("Ready failed: %s"), *ErrorResponse.Detail);
}
```

Startup order summary:

1. `GetSubsystem<...>()` on each of the three subsystems.
2. `AddDynamic` the allocate / deallocate handlers.
3. `SubscribeToServerEvents()` to open the SDK-daemon connection.
4. Configure the query handler reported values, then `Connect()`.
5. `ReadyServerForPlayers()` last — once everything else is live.

Teardown is the reverse: `Disconnect()` the query handler, then
`UnsubscribeToServerEvents()`, in `Shutdown()`.

## Launch parameters

The recommended launch parameters for Unreal servers on Multiplay Hosting:

```
-log=$$serverid$$/$$log_dir$$/$$serverid$$-$$timestamp$$.log -port=$$port$$
```

| Parameter | Purpose |
| ----------- | ----------- |
| `-log` | Log file path, resolved **relative to the Unreal `Saved` directory**. Use this when you want Unreal to manage the log location. |
| `-ABSLOG` | Log file path, resolved as an **absolute path**. Use this when you need the log written to a specific location, such as `$$log_dir$$` so it is exposed via the Multiplay Dashboard. |
| `-port` | Game port the server binds for player traffic. Parsed by Unreal Engine itself (NetDriver `PortOverride`). |

Variables wrapped in `$$...$$` are resolved by Multiplay Hosting at server
start. Set the launch parameters on your build configuration in the Multiplay
Dashboard.

* If you add a custom configuration variable on your build configuration's server.json, such
  as `mode: $$mode$$`, it will land in `server.json` as `"mode": "<n>"`. Custom variables are the recommended
  way to surface additional runtime values to your server (see
  [Extending server.json with custom configuration variables](#extending-serverjson-with-custom-configuration-variables)).
* The same equivalence applies to `$$query_port$$`, `$$serverid$$`,
  `$$log_dir$$`, and every other Multiplay-substituted variable.
* Treat the launch parameters/commandline args and the `server.json` field as two views of the
    same platform-supplied value; use whichever is more convenient for the code
    path in question.


### Log path best practice

Use a unique log filename per server instance *and* per restart. The
recommended pattern combines the server ID with the start timestamp:

```
$$serverid$$/$$log_dir$$/$$serverid$$-$$timestamp$$.log
```

This gives each server its own directory and each restart its own file, so logs
are not overwritten when a server process restarts (for example, after a crash
or a build rollout). Use this pattern with either `-log` or `-ABSLOG`:

```
# Relative to Saved/
-log=$$serverid$$/$$log_dir$$/$$serverid$$-$$timestamp$$.log

# Absolute path, written under the Multiplay-managed log directory
-ABSLOG=$$serverid$$/$$log_dir$$/$$serverid$$-$$timestamp$$.log
```

Refer to the [Multiplay Hosting documentation on redirecting Unreal logs][unreal-logs]
for more detail.

[unreal-logs]: https://open-2v.gitbook.com/multiplay-customer-docs/debugging/redirect-log-output-for-games-using-unreal

## Local testing and debugging

### Running the server locally without Multiplay infrastructure

On real Multiplay-hosted machines, Multiplay Hosting writes `server.json` and
runs the local SDK daemon (`sdkdaemon`) on port `8086`. Neither exists on a
developer workstation, so a default SDK run outside Multiplay will fail to
parse the config and fail to connect its event channel.

To iterate locally, drop a mock `server.json` at the location the SDK expects:

* **Linux:** `$HOME/server.json`
* **Windows:** `%HOMEPATH%/server.json`

Minimum fields (match the SDK's parser — refer to the
[Server Config fields](#server-config-subsystem) table for the full list):

```json
{
    "allocatedUUID": "",
    "serverID": "12345",
    "port": "9000",
    "queryPort": "9010",
    "ip": "127.0.0.1",
    "queryType": "sqp",
    "serverLogDir": "./logs/"
}
```

With that file in place, `URSMultiplayServerConfigSubsystem::GetServerConfig()`
returns sensible values and `URSMultiplayServerQueryHandlerSubsystem::Connect()`
binds SQP on `9010`. You can exercise the local handler with any SQP or
`go-svrquery` client.

Limitations of the mock-file approach:

* `URSMultiplayGameServerSubsystem::SubscribeToServerEvents()` opens a
  WebSocket to `ws://127.0.0.1:8086`. Without the SDK daemon running the
  connection fails and you will not receive `OnAllocate` / `OnDeallocate`
  events. Gate your subscription call behind a feature flag during local
  testing, or run the daemon if you have a development copy.
* `GetPayloadAllocation` hits the same local daemon. Mock responses by
  running a small HTTP server on port `8086` if you need to exercise the
  payload path.
* Populating `allocatedUUID` in the mock file does **not** fire
  `OnAllocate` — events come from the daemon's publication channel, not
  from the file. Use the `URSMultiplayServerConfigSubsystem` directly to
  simulate an allocation for code that only cares about the ID.

### Logging

The SDK uses the `LogRSMultiplayGameServerSDK` log category. It emits at
`Log`, `Verbose`, and `Error` levels covering subsystem lifecycle, HTTP
request/response traffic against the local daemon, SQP socket bind and
disconnect, allocation events, and ready/unready responses.

Bump it to `Verbose` when diagnosing integration issues:

* **Via command line:** add `-LogCmds="LogRSMultiplayGameServerSDK Verbose"`
  to the server launch parameters (either in the Multiplay Dashboard build
  configuration for cloud runs, or your local launch script).
* **Via config:** add the following to `DefaultEngine.ini`:

  ```ini
  [Core.Log]
  LogRSMultiplayGameServerSDK=Verbose
  ```

Healthy boot output includes a "Listening on port 'NNNN'" line once the SQP
socket binds. A "Failed to bind socket to port 'NNNN'" line at `Error` means
the query handler lost the port — most commonly due to the collision
scenario described in the
[binding port warning](#binding-port-and-command-line-overrides).

## Testbed project

A sample testbed project demonstrating QoS and direct connect flows is
documented separately. See [TESTBED.md](TESTBED.md).

## Appendix

### Required Visual Studio modules for Unreal Engine

- Microsoft.VisualStudio.Component.VC.Tools.x86.x64
- Microsoft.VisualStudio.Workload.ManagedDesktop
- Microsoft.VisualStudio.Workload.NativeDesktop
- Microsoft.VisualStudio.Workload.NativeGame
- Microsoft.Component.MSBuild
- Microsoft.NetCore.Component.Runtime.3.1
- Microsoft.Net.Component.4.6.2.TargetingPack
- Microsoft.Net.Component.4.7.2.TargetingPack
- Microsoft.VisualStudio.Component.VC.DiagnosticTools
- Microsoft.VisualStudio.Component.VC.ASAN
- Microsoft.VisualStudio.Component.Windows10SDK.19041
- Component.Unreal
