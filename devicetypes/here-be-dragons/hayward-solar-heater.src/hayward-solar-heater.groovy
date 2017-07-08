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

        attribute "speedNum", "number"
        attribute "overrideEnds", "string"
        attribute "currentSpeed", "number"
        attribute "currentStatus", "string"
        attribute "wattage", "number"
        attribute "flow", "number"

        command "sendCommand", ["number"]
        command "setSpeed", ["number"]
        command "speed1"
        command "speed2"
        command "speed3"
        command "speed4"
        command "speed5"
        command "speed6"
        command "speed7"
        command "speed8"
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
    	
    	valueTile("icon", "device.currentSpeed", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue} ', icon:"st.Weather.weather14")
		}
        multiAttributeTile(name:"temperature", type:"generic", width: 6, height: 4) {
        	tileAttribute("device.poolTemp", key: "PRIMARY_CONTROL") {
                attributeState "default", label:'${currentValue}',unit:"dF",
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
        standardTile("bRefresh", "device.switch", decoration: "flat", width: 2, height: 1) {
			state("default", action:"refresh.refresh", label:'Refresh', icon:"st.secondary.refresh")
		}
		standardTile("bResume", "device.auto", decoration: "flat", width: 2, height: 1) {
			state("default", action:"setAuto", label:'Resume')
		}
        valueTile("dSolarTemp", "device.label", decoration: "flat", width: 2, height: 1) {
        	state("default", label: 'Solar')
        }
        valueTile("dPoolTemp", "device.label", decoration: "flat", width: 2, height: 1) {
        	state("default", label: 'Pool')
        }
        valueTile("dRoofTemp", "device.label", decoration: "flat", width: 2, height: 1) {
        	state("default", label: 'Roof')
        }
        valueTile("bSolarTemp", "device.solarTemp", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue}',unit:"dF",
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
			state("default", label:'${currentValue}',unit:"dF",
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
        valueTile("bPoolTemp", "device.poolTemp", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue}',unit:"dF",
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
        standardTile("bState", "device.switch", decoration: "flat", width: 2, height: 2) {
			state("off", action:"switch.on", label:'off', icon:"st.switches.switch.off", backgroundColor:"#ffffff")
            state("on", action:"switch.off", label:'on', icon:"st.switches.switch.on", backgroundColor:"#00a0dc")
            state("auto", action:"switch.off", label:'on', icon:"st.switches.switch.on", defaultState: true)
        }

		main ("icon")   
		details([
        "temperature",
        "dPoolTemp",   "dSolarTemp",   "dRoofTemp",
		"bPoolTemp",   "bSolarTemp",   "bRoofTemp",
		"bState",      "bRefresh",
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
def poll() {
	log.debug "Executing 'poll'"
	getValues()
}

def speed1() {
	log.debug "Executing 'speed1'"
	setSpeed(1)
}

def speed2() {
	log.debug "Executing 'speed2'"
	setSpeed(2)
}

def speed3() {
	log.debug "Executing 'speed3'"
	setSpeed(3)
}

def speed4() {
	log.debug "Executing 'speed4'"
	setSpeed(4)
}

def speed5() {
	log.debug "Executing 'speed5'"
	setSpeed(5)
}

def speed6() {
	log.debug "Executing 'speed6'"
	setSpeed(6)
}

def speed7() {
	log.debug "Executing 'speed7'"
	setSpeed(7)
}

def speed8() {
	log.debug "Executing 'speed8'"
	setSpeed(8)
}

def speedResume() {
	log.debug "Executing 'Resume'"
	sendSpeed("0~0~0") // Third value indicates a "Running Schedule"
}

def refresh() {
	log.debug "Executing 'refresh'"
	getValues()
}

def modeOverride() {
	log.debug "Setting Perm. Override"
    sendSpeed("${state.speedNum}~0~2") // Third value indicates a "Permanent Override"
}

def on() {
	log.debug "Executing 'on'"
	speedResume()
}

def off() {
	log.debug "Executing 'off'"
	sendSpeed("1~0~2") // Permanent Override to Speed 1
}

def setSpeed(value) {
	log.debug "Executing sendCommand in setSpeed"
    log.debug "state.speedNum ${state.speedNum}"
    log.debug "new speed ${value}"
    if ( state.speedNum != value ) { // New speed selection
    	if ( state.mOverride != 2 ) { // works for new override or existing timed override
            log.debug "Timed Override of speed to ${value} for 60 minutes"
            sendSpeed("${value}~3600~1")
        } else { // perm override
            log.debug "Perm Override of speed ${value}"
            sendSpeed("${value}~0~2")
        }
    } else {
    	log.debug "Adding 60 minutes to manual speed"
		sendSpeed("0~3600~1") // 0 is add time, 3600 is 60 minutes, 1 is timed override
    }
}

// Make a call to the Pool Controller
private sendSpeed(command) {
	log.debug "Sending command ${command}"
    //Particle API Call
    def speedClosure = { response ->
	  	log.debug "Speed Request was successful, $response.data"
      	sendEvent(name: "currentSpeed", value: response.data.return_value)
        getValues()
	}
    
    def speedParams = [
  		uri: "https://api.particle.io/v1/devices/${deviceId}/mOverride",
        body: [access_token: token, command: command],  
        success: speedClosure
	]

	httpPost(speedParams)
}

def getValues(){
	
	httpGet(uri: "https://api.particle.io/v1/devices/${deviceId}/pumpSpeed?access_token=${token}",
    	contentType: 'application/json')
    {resp ->
            log.debug "resp data: ${resp.data}"
            log.debug "result: ${resp.data.result}"
		sendEvent(name: "currentSpeed", value: "${resp.data.result}")
	}
    httpGet(uri: "https://api.particle.io/v1/devices/${deviceId}/valuesST?access_token=${token}",
    	contentType: 'application/json')
    {resp ->
            log.debug "resp data: ${resp.data}"
            log.debug "result: ${resp.data.result}"
            def tokenValues = resp.data.result.tokenize('~')
            
            state.mOverride = tokenValues[0] as Integer;
            state.overrideEnds = tokenValues[1] as String;
            state.speedNum = tokenValues[2] as Integer;
            state.currentSpeed = tokenValues[3] as Integer;
            state.wattage = tokenValues[4] as Integer;
            state.flow = tokenValues[5] as Integer;
	}
    setStates()
}
def setStates(){
    log.debug "Setting speed activeS${state.speedNum} active"
    sendEvent(name: "setSpeed", value: "activeS${state.speedNum}", isStateChange: true, display: false)
    if (state.mOverride == 0) {
        log.info "Override: not enabled"
        sendEvent(name: "currentStatus", value: "Running Schedule", isStateChange: true, display: true)
    } else if (state.mOverride == 1) {
        log.info "Override: temporary schedule"
        sendEvent(name: "currentStatus", value: "Temporary Override until ${state.overrideEnds}", isStateChange: true, display: true)
    } else if (state.mOverride == 2){
        log.info "Override: permanent"
        sendEvent(name: "currentStatus", value: "Permanent Override", isStateChange: true, display: true)
    } else {
        log.info "Override: logic broken"
    }
}