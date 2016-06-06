<span id="PoolController.xhtml"></span>

<span>Automated Pool Controller</span>

<span>Timer Control of a Hayward Tristar VS Pump and Hayward GL-235 Solar Pool Controller</span>

<span>Overview</span>
---------------------

<span class="c10">Background:</span>

<span>Due to the engineering principle known as the Pump Affinity Law (</span> <span class="c14"> <a href="https://www.google.com/url?q=https://en.wikipedia.org/wiki/Affinity_laws&amp;sa=D&amp;ust=1465239922628000&amp;usg=AFQjCNFF25yA6eIEpKSKu_SDbKAkHEeJjg" class="c13">https://en.wikipedia.org/wiki/Affinity_laws</a> </span> <span>), a very significant energy savings can be had by reducing the speed at which you run your pool pump (approaching 90% savings in an optimal configuration vs. a single-speed pump).  The ideal configuration would be to run the pump 24-hours a day at the lowest setting possible to still achieve desired water turnover (rule of thumb: 1 water turnover per day), with needed speed-ups at various points during your day to enable the skimmer/vacuum/etc to function.</span>

<span></span>

<span>My solar heat controller, the Hayward GL-235, actuates a valve to enable or disable the solar panels based on the delta of water temp vs. roof temp, and a maximum temperature setting on the controller itself.  Additionally, according to Hayward, minimum flow once prime is achieved is 5GPM per panel.  I have 9 panels, so an optimal flow through the solar collectors is 45GPM.</span>

<span></span>

<span>I do not have a Vacuum system, or other systems that require specific or minimum speed settings.</span>

<span></span>

<span class="c10">The Problem:</span>

<span>You never know when solar heating will be available.  One day the controller could be calling for heat from 10am to 5pm, and the next it could never reach the delta needed to activate.  Therefore, we need a way to alert the pump to run at the minimum specified speed.</span>

<span></span>

<span class="c10">The Solution:</span>

<span>An arduino-based cloud device called the Particle Photon, with a break-out relay board to activate each of the 3 relays to control speed.</span>

<span>Code requirements:</span>

-   <span>Easily modified timing</span>
-   <span>Interruptible via buttons+display at controller and/or via phone app (SmartThings)</span>
-   <span>Automatic schedule recovery after a power outage</span>
-   <span>Automatic speed for Solar heater</span>

<span></span>

<span>The Hayward Tristar VS pool pump has a “third party controls” function that allows you to request a speed based on a combination of 3 relays.</span> <span>  Additionally, the Hayward GL-235 has a relay intended to to trigger a booster pump.  </span> <span>Using both of these facts.</span>

<span></span>

<span>![Alt text](/images/Tristar_VS_Relay_Wiring.png?raw=true "Pool Pump Diagram")</span>

<span></span>

<a href="" id="PoolController.xhtml#t.178c0836d2bf0d8807a0271b3fe88210c9d8df68"></a> <a href="" id="PoolController.xhtml#t.0"></a>

<table>
<colgroup>
<col width="25%" />
<col width="25%" />
<col width="25%" />
<col width="25%" />
</colgroup>
<tbody>
<tr class="odd">
<td><p><span class="c5">Timer Speed</span></p></td>
<td><p><span class="c5">STEP 1 Status</span></p></td>
<td><p><span class="c5">STEP 2 Status</span></p></td>
<td><p><span class="c5">STEP 3 Status</span></p></td>
</tr>
<tr class="even">
<td><p><span class="c5">1</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
</tr>
<tr class="odd">
<td><p><span class="c5">2</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
</tr>
<tr class="even">
<td><p><span class="c5">3</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
</tr>
<tr class="odd">
<td><p><span class="c5">4</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
</tr>
<tr class="even">
<td><p><span class="c5">5</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">ON</span></p></td>
</tr>
<tr class="odd">
<td><p><span class="c5">6</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">ON</span></p></td>
</tr>
<tr class="even">
<td><p><span class="c5">7</span></p></td>
<td><p><span class="c5">OFF</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">ON</span></p></td>
</tr>
<tr class="odd">
<td><p><span class="c5">8</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">ON</span></p></td>
<td><p><span class="c5">ON</span></p></td>
</tr>
</tbody>
</table>

<span> </span>

<span>The STEP inputs have very wide allowed voltages per the User Manual:</span>

<span>“Inputs are rated to accept a low voltage supply of 18-30VAC (24VAC+/-20%) or 9-30VDC (12/24VDC +/-20%).”</span>

<span>Hayward recommends taking power from +12V output to pump controller on RS485 connection as relay source power. “ICOM” (ground) would be jumpered to COM on RS485, so 4-wire connection to the Particle Photon would be:</span>

1.  <span>+12VDC (RS485) &lt;= (White wire) =&gt; Relay inputs</span>
2.  <span>STEP 1 (Relay) &lt;= (Blue wire) =&gt; Relay output</span>
3.  <span>STEP 2 (Relay) &lt;= (Brown wire) =&gt; Relay output</span>
4.  <span>STEP 3 (Relay) &lt;= (Black wire) =&gt; Relay output
    With a 5th internal jumper:</span>
5.  <span>RS485 COM &lt;= =&gt; Digital Input ICOM</span>

<span>(My 4/c wire is white/blue/brown/black)</span>

<span></span>

<span>From the Photon, I have a wire that runs through the booster pump relay (which I do not use for that) back to an INPUT pin with pulldown circuitry.  This allows the Photon to know when the solar collectors are active and react accordingly.</span>

<span></span>

<span>Lastly, every 5 minutes I output 4 separate metrics via a JSON Webhook to ThingSpeak:</span>

-   <span>Speed</span>
-   <span>Wattage</span>
-   <span>Flow</span>
-   <span>Solar activity</span>

<span></span>

<span>The first 3 of these are reliant on correct information entered into the top of the Photon’s code.</span>

<span></span>

<span>Parts:</span>
-------------------

-   <span>5v-controlled relay with at least 3 separate relay ports (something like SainSmart 4-Channel Relay Module, </span> <span class="c14"> <a href="https://www.google.com/url?q=http://www.amazon.com/SainSmart-101-70-101-4-Channel-Relay-Module/dp/B0057OC5O8&amp;sa=D&amp;ust=1465239922661000&amp;usg=AFQjCNFwsijnQRX7gYUiGtQefEgJ-vnOTQ" class="c13">http://www.amazon.com/SainSmart-101-70-101-4-Channel-Relay-Module/dp/B0057OC5O8</a> </span> <span>)</span>
-   <span>Particle Photon (</span> <span class="c14"> <a href="https://www.google.com/url?q=https://www.adafruit.com/product/2721&amp;sa=D&amp;ust=1465239922662000&amp;usg=AFQjCNFkvjlNffMXXw9yl_BdsKeojkoYew" class="c13">https://www.adafruit.com/product/2721</a> </span> <span>)</span>
-   <span>22AWG 4/C 300v wire (Connected to Pump)</span>
-   <span>22AWG 2/C 300v wire (Connected to Solar Controller)</span>
-   <span>0.96” 128x64 SPI/I</span> <span class="c8">2</span> <span>C OLED Display Module for Arduino (aliexpress.com)</span>

<span></span>

<span>Configuration of Script:</span>
-------------------------------------

<span class="c10">speedRPM\[8\]:</span> <span> Make necessary configurations via the pump controls for speeds you chose, then update the script with the pump’s speeds.  This ensures proper display values on the OLED, and to any web interfaces/dashboards.</span>

<span class="c10">energyConsum\[8\]:</span> <span> This is variable based on your installation (head pressure, pipe diameter, etc).  Trigger each configured speed and allow it to stabilize, then record the pump display’s wattage indication.</span>

<span class="c10">flowCalc\[8\]:</span> <span> If you have a flow meter, great, use that.  If not, an estimation can be made using mas985’s spreadsheet calculator from the Trouble Free Pools forum.  You can get the latest copy from the below URL.  You want “Pool Pump Tools”.</span>

<span class="c14"> <a href="https://www.google.com/url?q=http://www.troublefreepool.com/threads/830-Hydraulics-101-Have-you-lost-your-head?p%3D6231%26viewfull%3D1%23post6231&amp;sa=D&amp;ust=1465239922666000&amp;usg=AFQjCNEafl3629AkRvFhK31P9H_U7_XJfw" class="c13">http://www.troublefreepool.com/threads/830-Hydraulics-101-Have-you-lost-your-head?p=6231&amp;viewfull=1#post6231</a> </span>

<span>Other Notes:</span>
-------------------------

<span>GL-235 Specifics:</span>

<span></span>

<span>Do not use the relay on the right side, instead use the one designed for starting a booster pump.  The one on the right side injects 20VAC into the line.</span>

<span></span>

<span></span>

<span></span>
