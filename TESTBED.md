# Multiplay SDK for Unreal — Testbed

The testbed is a sample Unreal project used to validate the Multiplay SDK for
Unreal end-to-end: building client and server targets, uploading a build to
Multiplay Hosting, exercising the QoS flow, and connecting a client to an
allocated server via direct connect.

## Prerequisites

* Unreal Engine: Source Build (minimum version 4.26.2, verified up to 5.1.1)
* Operating System: Windows 10 / 11 or Ubuntu / Debian
* IDE: Visual Studio 2019 or 2022 (recommended for UE 5.x)
* Linux Toolchain: `v20_clang-13.0.1-centos7` (required for cross-compiling UE 5.1 targets)

## Build the executables

Build the executables for both the client and server. The first iteration of
these builds with a source build of Unreal may take a few hours depending on
machine specifications.

### Unreal Client

#### Unreal Engine 5

1. Open the project.
2. In the top toolbar select **Platforms > Windows** or **Linux**.
3. Ensure the following options are selected:
    * Binary Configuration: **Development**
    * Build Target: **TestBed**
4. Click **Package Project**.

#### Unreal Engine 4

1. Open the project.
2. Navigate to **File > Package Project**.
3. Select **Build Target > TestBed**.
4. Select your platform:
    * Windows (64-bit)
    * Linux

### Unreal Server

#### Unreal Engine 5

1. Open the project.
2. Navigate to **Platforms > Linux**.
3. Ensure the following options are selected:
    * Binary Configuration: **Development**
    * Build Target: **TestBedServer**
4. Click **Package Project**.

#### Unreal Engine 4

1. Open the project.
2. Navigate to **File > Package Project**.
3. Select **Build Target > TestBedServer**.
4. Select **Linux**.

## Configure Multiplay Hosting

Upload the server build to the Multiplay Dashboard and create a build
configuration linked to it.

### Example launch parameters

```
-nosound -nullrhi -port=$$port$$ -queryport=$$query_port$$ -queryType=$$query_type$$ -log=$$serverid$$/$$timestamp$$-$$serverid$$.log
```

> **Note:** The first time the build is uploaded it may take up to 5 minutes to
> sync.

### Create a fleet

Create a fleet for this build configuration in your local region.

## QoS

### How to run

1. Run a client build or run the project in the editor.
2. Press the **QoS** button.
3. Enter the fleet ID of your QoS server.
4. Click **Request**.

### Example expected successful logs

```
Region: XXXXXXXXXXXX
Latency: 183
Packet Loss: 0
```

### Example unsuccessful logs

```
QoS Error: Invalid server response when trying to get servers: Fleet not found
```

## Direct Connect

Provides a basic UI to connect a client to a server via direct IP and port.

### In Editor

1. Run the project in the Unreal Editor.
2. Press the **Direct Connect** button.
3. Enter:
    * Server IP
    * Server Port
4. Click **Connect**.

If successful, the client will connect to the hosted server and transition into
the game map.

### From client build

1. Launch the client build.
2. Press the **Direct Connect** button.
3. Enter:
    * Server IP
    * Server Port
4. Click **Connect**.

If successful, the client will connect to the hosted server and transition into
the Third Person Map.

### Example expected successful logs

```
OnReadyServerSuccess - Server is now accepting players
```
