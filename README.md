# Automated Pool Controller
###### Timer Control of a Hayward Tristar VS Pump and Hayward GL-235 Solar Pool Controller
![Alt text](/images/pool_controller.jpg?raw=true "Finished Pool Controller")  
![Alt text](/images/grafana.png?raw=true "Grafana Dashboard")
### Overview
---------------------
**Background:**
Due to the engineering principle known as the Pump Affinity Law (<https://en.wikipedia.org/wiki/Affinity_laws>), a very significant energy savings can be had by reducing the speed at which you run your pool pump (approaching 90% savings in an optimal configuration vs. a single-speed pump).  The ideal configuration for energy savings would be to run a pump 24-hours a day at the lowest setting possible to still achieve desired water pumping volume (rule of thumb: 1 pool's water turnover per day), with needed speed-ups at various points during your day to enable the skimmer/vacuum/etc to function.  

My solar heat controller, the Hayward GL-235, actuates a valve to enable or disable the solar panels based on the delta of water temp vs. roof temp, and a maximum temperature setting on the controller itself.  Additionally, according to Hayward, minimum flow for optimal water heating once prime is achieved is 5GPM per panel.  I have 9 panels, so an optimal flow through the solar collectors is 45GPM.  

I do not have a Vacuum system, or other systems that require specific or minimum speed settings.

**The Problem:**
You never know when solar heating will be available.  One day the controller could be calling for heat from 10am to 5pm, and the next it could never reach the delta needed to activate.  Therefore, we need a way to alert the pump to run at the minimum specified speed.

**The Solution:**
An arduino-based butt device called the Particle Photon, with a break-out relay board to activate each of the 3 relays to control speed.  

Code requirements:
-   Easily modified timing
-   Interruptible via buttons+display at controller and/or via phone app (SmartThings)
-   Automatic schedule recovery after a power outage
-   Automatic speed for Solar heater
-   Tracking of a number of values related to the pool:
    -   Water temperature before solar heating
    -   Water temperature after solar heating
    -   Roof temperature
    -   Total kWh used per day
    -   Total gallons pumped per day

The Hayward Tristar VS pool pump has a "third party controls" function that allows you to request a speed based on a combination of 3 relays.  Additionally, the Hayward GL-235 has a relay intended to to trigger a booster pump.  Using both of these facts:  

![Alt text](/images/Tristar_VS_Relay_Wiring.png?raw=true "Pool Pump Diagram")

| Timer Speed | STEP 1 Status | STEP 2 Status | STEP 3 Status |
| ----------- | ------------- | ------------- | ------------- |
| 1           | OFF           | OFF           | OFF           |
| 2           | ON            | OFF           | OFF           |
| 3           | OFF           | ON            | OFF           |
| 4           | ON            | ON            | OFF           |
| 5           | OFF           | OFF           | ON            |
| 6           | ON            | OFF           | ON            |
| 7           | OFF           | ON            | ON            |
| 8           | ON            | ON            | ON            |

The STEP inputs have very wide allowed voltages per the User Manual:
> Inputs are rated to accept a low voltage supply of 18-30VAC (24VAC+/-20%) or 9-30VDC (12/24VDC +/-20%).

Hayward recommends taking power from +12V output to pump controller on RS485 connection as relay source power. "ICOM" (ground) would be jumpered to COM on RS485, so 4-wire connection to the Particle Photon would be:
1.  +12VDC (RS485) &lt;= (White wire) =&gt; Relay inputs
2.  STEP 1 (Relay) &lt;= (Blue wire) =&gt; Relay output
3.  STEP 2 (Relay) &lt;= (Brown wire) =&gt; Relay output
4.  STEP 3 (Relay) &lt;= (Black wire) =&gt; Relay output
    With a 5th internal jumper:
5.  RS485 COM &lt;= =&gt; Digital Input ICOM

(My 4/c wire is white/blue/brown/black)  

From the Photon, I have a wire that runs through the Solar Controller's booster pump relay (I do not have a booster pump) back to an INPUT pin with pulldown circuitry.  This allows the Photon to know when the solar collectors are active and react accordingly.  

Lastly, I have a number of variables made available for my Grafana dashboard with metrics gathered by Collectd:
-   Speed
-   Wattage
-   Flow
-   Solar activity
-   Temperature of pool
-   Temperature after solar
-   Temperature of roof

The first 3 of these are reliant on correct tables in the top of the Photon's code.

### Parts:
-------------------
-   5v-controlled relay with 4 separate relay ports (something like SainSmart 4-Channel Relay Module, <http://www.amazon.com/SainSmart-101-70-101-4-Channel-Relay-Module/dp/B0057OC5O8>)
-   Particle Photon
-   22AWG 4/C 300v wire (Connected to Pump)
-   22AWG 2/C 300v wire (Connected to Solar Controller)
-   0.96" 128x64 SPI/I<sup>2</sup>C OLED Display Module for Arduino (aliexpress.com)
-   Custom printed PCB from Osh Park: https://oshpark.com/shared_projects/12k2QJzh
-   10k photoresistor
-   4 2-pin headers
-   4 push buttons
-   Barrel jack connector
-   Various resistors (identified in eagle/Pool_Controller.sch)


### Configuration of code:
-------------------------------------
**speedRPM\[8\]:** Make necessary configurations via the pump controls for speeds you chose, then update the code's array with the pump's speeds.  This ensures proper display values on the OLED, and to any web interfaces/dashboards.

**energyConsum\[8\]:** This is variable based on your installation (head pressure, pipe diameter, etc).  Trigger each configured speed and allow it to stabilize, then record the pump display’s wattage indication to the code's array.

**flowCalc\[8\]:** If you have a flow meter, great, use that.  If not, an estimation can be made using mas985's spreadsheet calculator from the Trouble Free Pools forum.  You can get the latest copy from the below URL.  You want "Pool Pump Tools".
<http://www.troublefreepool.com/threads/830-Hydraulics-101-Have-you-lost-your-head?p=6231&amp;viewfull=1#post6231>

### Other Notes:
-------------------------
**GL-235 Specifics:**  
Do not use the relay on the right side, instead use the one designed for starting a booster pump.  The one on the right side injects 20VAC into the line.

**Data Tracking:**
I have data tracked/graphed using the following:
-   Grafana + InfluxDB + Collectd:
    -   Collectd uses the `curl_json` plugin to query the `pumpSpeed`, `pumpWattage`, `waterFlow`, `solarAct`, and `override` variables
    -   Collectd uses the `network` plugin to output the results to an InfluxDB TSD
    -   Grafana displays the results on a dashboard.
    -   This requires a Linux device running somewhere.  It does not need to have any ports accessible to the internet since it is pulling the data.
    -   See collectd/pool-controller.conf and grafana/pool-controller.json for relevant configuration examples.
-   Google Documents: Particle.Publish() -> IFTTT Recipe -> Google Drive
    -   Recipe example:
        -   If (Event Name): `24Hr_kWh`
        -   is (Event Contents): <Leave Blank>
        -   Device Name or ID: Particle Device being used
        -   Add row to spreadsheet
    -   This is done to track daily power usage and daily gallons pumped.
