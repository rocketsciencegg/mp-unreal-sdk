# Unreal Multiplay SDK
mp-unreal-sdk

## Prerequisites

* Source Build of Unreal Engine (recommended version 4.26.2)
* Windows 10 / 11 or Ubuntu / Debian operating system
* Visual Studio 2017 or later


Required modules are listed in the appendix.

## Build the Executables
Build the executables for both the client and server. The first iteration of these builds with a source build of Unreal may take a few hours depending on machine specifications.

---

### Unreal Client
#### Unreal Engine 5

1. Open the project.
2. In the top toolbar select:<br>
    a. Platforms -> Windows or Linux
3. Ensure the following options are selected:<br>
    a. Binary Configuration: Development<br>
    b. Build Target: TestBed
5. Click Package Project.

#### Unreal Engine 4
1. Open the project.
2. Navigate to:<br>
    a. File -> Package Project
3. Select:<br>
    a. Build Target -> TestBed
4. Select your platform:<br>
    a. Windows (64-bit)<br>
    b. Linux

### Unreal Server
#### Unreal Engine 5

1. Open the project.
2. Navigate to:<br>
    a. Platforms -> Linux
3. Ensure the following options are selected:<br>
    a. Binary Configuration: Development<br>
    b. Build Target: TestBedServer
4. Click Package Project.

#### Unreal Engine 4
1. Open the project.
2. Navigate to:<br>
    a. File -> Package Project
3. Select:<br>
    a. Build Target -> TestBedServer
4. Select Linux.


## Configure Multiplay
Upload the server build to the Multiplay build configuration.
After uploading, create a Build Configuration

### Example Launch Parameters
-nosound -nullrhi -port=$$port$$ -queryport=$$query_port$$ -queryType=$$query_type$$ -LOG=../../../../$$log_dir$$/$$serverid$$.log
Note: The first time the build is uploaded it may take up to 5 minutes to sync.

### Create Fleet
Create a Fleet for this Build Configuration in your local region.

## QoS
### How to Run
1. Run a client build or run the project in the editor.
2. Press the QoS button.
3. Enter the Fleet ID of your QoS server.
4. Click Request.


### Example Expected Successful Logs
Region: XXXXXXXXXXXX
Latency 183
Packet Loss: 0


### Example Unsuccessful Logs
QoS Error: Invalid server response when trying to get servers: Fleet not found







## Direct Connect
### Purpose
Provide a basic UI to connect a client to a server via direct IP and port.

---

Run one or more client applications and start matchmaking.

### In Editor
1. Run the project in the Unreal Editor.
2. Press the Direct Connect button.
3. Enter:
    * Server IP
    * Server Port
4. Click Connect.
5. 
If successful, the client will connect to the hosted server and transition into the game map.

### From Client Build

1. Launch the client build.
2. Press the Direct Connect button.
3. Enter:
    * Server IP
    * Server Port
4. Click Connect.

If successful, the client will connect to the hosted server and transition into the Third Person Map.

### Example Expected Successful Logs
“OnReadyServerSuccess - Server is now accepting players”





## Appendix
Required Visual Studio Modules for UE
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

