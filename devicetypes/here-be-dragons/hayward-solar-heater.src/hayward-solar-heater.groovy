/*
 *  Hayward Solar Heater
 *  https://github.com/Here-Be-Dragons/Pool-Controller
 *  Copyright 2017 Jerad Jacob
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 */
metadata {
	definition (name: "Hayward Solar Heater", namespace: "Here-Be-Dragons", author: "Jerad Jacob") {
        capability "Actuator" //Best practice to include if it has commands
        capability "Sensor"  //Best practice to include if it has attributes
        capability "Polling"
        capability "Switch"
        capability "Refresh"
        capability "Relay Switch"

        attribute "poolTemp", "number"
        attribute "solarTemp", "number"
        attribute "roofTemp", "number"
        attribute "currentStatus", "string"

        command "sendCommand", ["number"]
        command "setSpeed", ["number"]
        command "auto"
        command "addTime"
        command "speedResume"
        command "modeOverride"
	}


	simulator {
		// TODO: define status and reply messages here
	}
    
    preferences {
        input("deviceId", "text", title: "Device ID", description: "Particle Device ID", required: false, displayDuringSetup: true)
        input("token", "text", title: "Access Token", description: "Particle User Access Token", required: false, displayDuringSetup: true)
    }

	tiles(scale: 2) {
    	
    	valueTile("icon", "device.poolTemp", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue} ', icon:"st.Weather.weather14",
            backgroundColors:[
                    [value: 31, color: "#153591"],
                    [value: 44, color: "#1e9cbb"],
                    [value: 59, color: "#90d2a7"],
                    [value: 74, color: "#44b621"],
                    [value: 84, color: "#f1d801"],
                    [value: 95, color: "#d04e00"],
                    [value: 96, color: "#bc2323"]
                ])
		}
        multiAttributeTile(name:"temperature", type:"generic", width: 6, height: 4) {
        	tileAttribute("device.poolTemp", key: "PRIMARY_CONTROL") {
                attributeState "default", label:'${currentValue}°F',unit:"dF",
                backgroundColors:[
                    [value: 31, color: "#153591"],
                    [value: 44, color: "#1e9cbb"],
                    [value: 59, color: "#90d2a7"],
                    [value: 74, color: "#44b621"],
                    [value: 84, color: "#f1d801"],
                    [value: 95, color: "#d04e00"],
                    [value: 96, color: "#bc2323"]
                ]
            }
            tileAttribute("device.currentStatus", key: "SECONDARY_CONTROL") {
            	attributeState "default", label:'Currently: ${currentValue}'
            }
        }
		standardTile("poll", "device.refresh", decoration: "flat", width: 2, height: 2) {
			state("default", action:"polling.poll", label:'Poll', icon:"st.secondary.refresh")
		}
        standardTile("bRefresh", "device.switch", decoration: "flat", width: 2, height: 2) {
			state("default", action:"refresh.refresh", label:'Refresh', icon:"st.secondary.refresh")
		}
        valueTile("dSolarTemp", "device.label", decoration: "flat", width: 2, height: 1) {
        	state("default", label: 'Solar')
        }
        valueTile("dAmbTemp", "device.label", decoration: "flat", width: 2, height: 1) {
        	state("default", label: 'Ambient')
        }
        valueTile("dRoofTemp", "device.label", decoration: "flat", width: 2, height: 1) {
        	state("default", label: 'Roof')
        }
        valueTile("bSolarTemp", "device.solarTemp", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue}°F',unit:"dF",
                backgroundColors:[
                    [value: 31, color: "#153591"],
                    [value: 44, color: "#1e9cbb"],
                    [value: 59, color: "#90d2a7"],
                    [value: 74, color: "#44b621"],
                    [value: 84, color: "#f1d801"],
                    [value: 95, color: "#d04e00"],
                    [value: 96, color: "#bc2323"]
                ])
        }
        valueTile("bRoofTemp", "device.roofTemp", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue}°F',unit:"dF",
                backgroundColors:[
                    [value: 31, color: "#153591"],
                    [value: 44, color: "#1e9cbb"],
                    [value: 59, color: "#90d2a7"],
                    [value: 74, color: "#44b621"],
                    [value: 84, color: "#f1d801"],
                    [value: 95, color: "#d04e00"],
                    [value: 96, color: "#bc2323"]
                ])
        }
        valueTile("bAmbTemp", "device.ambTemp", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue}°F',unit:"dF",
                backgroundColors:[
                    [value: 31, color: "#153591"],
                    [value: 44, color: "#1e9cbb"],
                    [value: 59, color: "#90d2a7"],
                    [value: 74, color: "#44b621"],
                    [value: 84, color: "#f1d801"],
                    [value: 95, color: "#d04e00"],
                    [value: 96, color: "#bc2323"]
                ])
        }
        standardTile("bState", "device.mode", decoration: "flat", width: 2, height: 2) {
			state("off", action:"switch.on", label:'off', icon:"st.Weather.weather14", backgroundColor:"#ffffff")
            state("on", action:"auto", label:'on', icon:"st.Weather.weather14", backgroundColor:"#bc2323")
            state("auto", action:"switch.off", label:'auto', icon:"st.Weather.weather14", defaultState: true)
        }

		main ("icon")   
		details([
        "temperature",
        "dSolarTemp",  "dRoofTemp",  "dAmbTemp",
		"bSolarTemp",  "bRoofTemp",  "bAmbTemp",
		"bState",      "bRefresh"
		])
	}
}

// parse events into attributes
def parse(String description) {
	log.debug "Parsing '${description}'"
	// TODO: handle 'switch' attribute
	// TODO: handle 'speedNum' attribute

}

// handle commands
def on() {
	log.debug "Executing 'on'"
	sendState(1)
}

def off() {
	log.debug "Executing 'off'"
	sendState(0)
}

def auto() {
	log.debug "Executing 'auto'"
	sendState(2)
}

def refresh() {
	log.debug "Executing 'refresh'"
	getValues()
}

// Make a call to the Pool Controller
private sendState(command) {
	log.debug "Sending command ${command}"
    //Particle API Call
    def solarClosure = { response ->
	  	log.debug "Update was successful, recieved: $response.data"
      	sendEvent(name: "poolTemp", value: response.data.return_value)
        getValues()
	}
    
    def sendParams = [
  		uri: "https://api.particle.io/v1/devices/${deviceId}/solarCmd",
        body: [access_token: token, command: command],  
        success: solarClosure
	]

	httpPost(sendParams)
}

def getValues(){

    httpGet(uri: "https://api.particle.io/v1/devices/${deviceId}/solarST?access_token=${token}",
    	contentType: 'application/json')
    {resp ->
            log.debug "resp data: ${resp.data}"
            log.debug "result: ${resp.data.result}"
            def tokenValues = resp.data.result.tokenize('~')
            
            state.sOverride = tokenValues[0] as Integer;
            state.sState = tokenValues[1] as Integer;
            state.poolTemp = (tokenValues[2] as Float).round(1);
            state.roofTemp = (tokenValues[3] as Float).round(0);
            state.solarTemp = (tokenValues[4] as Float).round(1);
            state.ambTemp = (tokenValues[5] as Float).round(0);
            
	}
    setStates()
}
def setStates(){
    log.debug "sOverride is ${state.sOverride} and sState ${state.sState}"
    if (state.sOverride == 0) { //Override enabled, solar off
        log.info "Override: YES, Solar: Not Heating"
        sendEvent(name: "currentStatus", value: "Not Heating (Override)", isStateChange: true, display: true)
        sendEvent(name: "mode", value: "off", isStateChange: true, display: false)
    } else if (state.sOverride == 1) { //Override enabled, solar on
        log.info "Override: YES, Solar: Heating"
        sendEvent(name: "currentStatus", value: "Heating (Override)", isStateChange: true, display: true)
        sendEvent(name: "mode", value: "on", isStateChange: true, display: false)
    } else if (state.sOverride == 2) { //Override disabled
        if (state.sState == 0) {
            log.info "Override: NO, Solar: Not Heating"
            sendEvent(name: "currentStatus", value: "Not Heating", isStateChange: true, display: true)
        } else if (state.sState == 1) {
            log.info "Override: NO, Solar: Heating"
            sendEvent(name: "currentStatus", value: "Heating", isStateChange: true, display: true)
        } else {
            log.info "BROKEN: cannot determine state for sOverride = 1"
        }
        sendEvent(name: "mode", value: "auto", isStateChange: true, display: false)
    } else {
    	log.info "BROKEN: did not enter if for any sOverrides in setStates"
    }
    sendEvent(name: "poolTemp", value: "${state.poolTemp}", isStateChange: true, display: false)
    sendEvent(name: "ambTemp", value: "${state.ambTemp}", isStateChange: true, display: false)
    sendEvent(name: "solarTemp", value: "${state.solarTemp}", isStateChange: true, display: false)
    sendEvent(name: "roofTemp", value: "${state.roofTemp}", isStateChange: true, display: false)
}