/**
 *  Hayward Solar Pool Controller
 *
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
	definition (name: "Hayward VS Pool Pump", namespace: "Here-Be-Dragons", author: "Jerad Jacob") {
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
			state("default", label:'${currentValue} RPM', icon:"st.Health & Wellness.health2")
		}
        multiAttributeTile(name:"speed", type:"generic", width: 6, height: 4) {
        	tileAttribute("device.currentSpeed", key: "PRIMARY_CONTROL") {
                attributeState "default", label:'${currentValue} RPM', backgroundColor:"#add8e6"
            }
            tileAttribute("device.currentStatus", key: "SECONDARY_CONTROL") {
            	attributeState "default", label:'Currently: ${currentValue}'
            }
        }
		standardTile("bSpeed1", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
            state ("activeS1", label: 'OFF', action:"speed1", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS2", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activeS3", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activeS4", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activeS5", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activeS6", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activeS7", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activeS8", label: 'OFF', action:"speed1", nextState: "activating")
            state ("inactive", label: 'OFF', action:"speed1", nextState: "activating")
            state ("activating", label: 'OFF', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
		standardTile("bSpeed2", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
            state ("activeS1", label: '600', action:"speed2", nextState: "activating")
            state ("activeS2", label: '600', action:"speed2", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS3", label: '600', action:"speed2", nextState: "activating")
            state ("activeS4", label: '600', action:"speed2", nextState: "activating")
            state ("activeS5", label: '600', action:"speed2", nextState: "activating")
            state ("activeS6", label: '600', action:"speed2", nextState: "activating")
            state ("activeS7", label: '600', action:"speed2", nextState: "activating")
            state ("activeS8", label: '600', action:"speed2", nextState: "activating")
            state ("inactive", label: '600', action:"speed2", nextState: "activating")
            state ("activating", label: '600', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
		standardTile("bSpeed3", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
			state ("activeS1", label: '1200', action:"speed3", nextState: "activating")
            state ("activeS2", label: '1200', action:"speed3", nextState: "activating")
            state ("activeS3", label: '1200', action:"speed3", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS4", label: '1200', action:"speed3", nextState: "activating")
            state ("activeS5", label: '1200', action:"speed3", nextState: "activating")
            state ("activeS6", label: '1200', action:"speed3", nextState: "activating")
            state ("activeS7", label: '1200', action:"speed3", nextState: "activating")
            state ("activeS8", label: '1200', action:"speed3", nextState: "activating")
            state ("inactive", label: '1200', action:"speed3", nextState: "activating")
            state ("activating", label: '1200', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
		standardTile("bSpeed4", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
			state ("activeS1", label: '1800', action:"speed4", nextState: "activating")
            state ("activeS2", label: '1800', action:"speed4", nextState: "activating")
            state ("activeS3", label: '1800', action:"speed4", nextState: "activating")
            state ("activeS4", label: '1800', action:"speed4", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS5", label: '1800', action:"speed4", nextState: "activating")
            state ("activeS6", label: '1800', action:"speed4", nextState: "activating")
            state ("activeS7", label: '1800', action:"speed4", nextState: "activating")
            state ("activeS8", label: '1800', action:"speed4", nextState: "activating")
            state ("inactive", label: '1800', action:"speed4", nextState: "activating")
            state ("activating", label: '1800', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
		standardTile("bSpeed5", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
			state ("activeS1", label: '2300', action:"speed5", nextState: "activating")
            state ("activeS2", label: '2300', action:"speed5", nextState: "activating")
            state ("activeS3", label: '2300', action:"speed5", nextState: "activating")
            state ("activeS4", label: '2300', action:"speed5", nextState: "activating")
            state ("activeS5", label: '2300', action:"speed5", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS6", label: '2300', action:"speed5", nextState: "activating")
            state ("activeS7", label: '2300', action:"speed5", nextState: "activating")
            state ("activeS8", label: '2300', action:"speed5", nextState: "activating")
            state ("inactive", label: '2300', action:"speed5", nextState: "activating")
            state ("activating", label: '2300', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
		standardTile("bSpeed6", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
			state ("activeS1", label: '2750', action:"speed6", nextState: "activating")
            state ("activeS2", label: '2750', action:"speed6", nextState: "activating")
            state ("activeS3", label: '2750', action:"speed6", nextState: "activating")
            state ("activeS4", label: '2750', action:"speed6", nextState: "activating")
            state ("activeS5", label: '2750', action:"speed6", nextState: "activating")
            state ("activeS6", label: '2750', action:"speed6", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS7", label: '2750', action:"speed6", nextState: "activating")
            state ("activeS8", label: '2750', action:"speed6", nextState: "activating")
            state ("inactive", label: '2750', action:"speed6", nextState: "activating")
            state ("activating", label: '2750', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
        standardTile("bSpeed7", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
			state ("activeS1", label: '3000', action:"speed7", nextState: "activating")
            state ("activeS2", label: '3000', action:"speed7", nextState: "activating")
            state ("activeS3", label: '3000', action:"speed7", nextState: "activating")
            state ("activeS4", label: '3000', action:"speed7", nextState: "activating")
            state ("activeS5", label: '3000', action:"speed7", nextState: "activating")
            state ("activeS6", label: '3000', action:"speed7", nextState: "activating")
            state ("activeS7", label: '3000', action:"speed7", backgroundColor:"#00FF00", nextState: "activating")
            state ("activeS8", label: '3000', action:"speed7", nextState: "activating")
            state ("inactive", label: '3000', action:"speed7", nextState: "activating")
            state ("activating", label: '3000', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
        standardTile("bSpeed8", "device.setSpeed", decoration: "flat", width: 2, height: 1) {
			state ("activeS1", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS2", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS3", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS4", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS5", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS6", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS7", label: '3450', action:"speed8", nextState: "activating")
            state ("activeS8", label: '3450', action:"speed8", backgroundColor:"#00FF00", nextState: "activating")
            state ("inactive", label: '3450', action:"speed8", nextState: "activating")
            state ("activating", label: '3450', backgroundColor:"#00a0dc")
            state ("disabled", label: '')
		}
		standardTile("poll", "device.refresh", decoration: "flat", width: 2, height: 2) {
			state("default", action:"polling.poll", label:'Poll', icon:"st.secondary.refresh")
		}
        standardTile("bRefresh", "device.switch", decoration: "flat", width: 2, height: 1) {
			state("default", action:"refresh.refresh", label:'Refresh', icon:"st.secondary.refresh")
		}
		standardTile("bResume", "device.resume", decoration: "flat", width: 2, height: 1) {
			state("default", action:"speedResume", label:'Resume')
		}
        standardTile("bOverride", "device.override", decoration: "flat", width: 4, height: 1) {
			state("default", action:"modeOverride", label:'Perm. Override')
		}

		main ("icon")   
		details([
        "speed",
        "bSpeed1",    "bSpeed2",    "bSpeed3",
		"bSpeed4",    "bSpeed5",    "bSpeed6",
		"bSpeed7",    "bSpeed8",    "bResume",
        "bOverride",  "bRefresh",
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