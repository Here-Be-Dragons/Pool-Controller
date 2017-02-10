/**
 * Hayward Tristar VS controller using Particle Photon
 *
 *  Copyright 2016 Jerad Jacob
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
	definition (name: "Hayward Tristar VS Controller", namespace: "Here-Be-Dragons", author: "Jerad Jacob") {
		capability "Actuator" //Best practice to include if it has commands
        capability "Sensor" //Best practice to include if it has attributes
        capability "Polling"
        capability "Refresh"
        capability "Relay Switch"
        
        attribute "speedNum", "string"
        
        command "sendCommand", ["number"]
        command "speed1"
        command "speed2"
        command "speed3"
        command "speed4"
        command "speed5"
        command "speed6"
        command "speed7"
        command "speed8"
        command "resume"
	}

	simulator {
		
	}
    
    preferences {
        input("deviceId", "text", title: "Device ID", description: "Particle Device ID", required: false, displayDuringSetup: true)
        input("token", "text", title: "Access Token", description: "Particle User Access Token", required: false, displayDuringSetup: true)
    }
    
    tiles(scale: 2) {
		valueTile("icon", "device.currentSpeed", decoration: "flat", width: 2, height: 2) {
			state("default", label:'${currentValue} RPM', icon:"st.Health & Wellness.health2")
		}
		valueTile("currentSpeed", "device.currentSpeed", decoration: "flat", width: 3, height: 2) {
			state("default", label:'Currently: ${currentValue} RPM')
		}
		standardTile("off", "device.setSpeed", decoration: "flat", width: 3, height: 1) { // This kinda works!!!
			state "inactive", label: 'OFF', action:"speed1", nextState: "active"
            state "active", label: 'OFF', action:"speed1", nextState: "inactive", backgroundColor:"#00FF00"
		}
		standardTile("speed2", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed2",  label:'600')
			state("active", action:"speed2", label:'600')
		}
		standardTile("speed3", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed3", label:'1200')
			state("active", action:"speed3", label:'1200')
		}
		standardTile("speed4", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed4", label:'1800')
			state("active", action:"speed4", label:'1800')
		}
		standardTile("speed5", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed5", label:'2300')
			state("active", action:"speed5", label:'2300')
		}
		standardTile("speed6", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed6",   label:'2750')
			state("active", action:"speed6", label:'2750')
		}
		standardTile("speed7", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed7", label:'Set Speed 7', backgroundColor:"#F7C4BA")
			state("active", action:"speed7", label:'3000')
		}
		standardTile("speed8", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed8", label:'Set Speed 8', backgroundColor:"#F7C4BA")
			state("active", action:"speed8", label:'3450')
		}
		standardTile("refresh", "device.refresh", decoration: "flat", width: 3, height: 1) {
			state("default", action:"polling.poll", label:'Refresh Current Speed', icon:"st.secondary.refresh")
		}
		standardTile("resume", "device.resume", decoration: "flat", width: 3, height: 1) {
			state("default", action:"speedResume", label:'Resume Schedule')
		}

		main ("icon")   
		details([
		"currentSpeed",	"refresh",
						"resume",
		"off",			"speed2",
		"speed3", 		"speed4",
		"speed5", 		"speed6",
		"speed7", 		"speed8"
		])
	}
}

// handle commands
def poll() {
	log.debug "Executing 'poll'"
    getSpeed(0)
}

def speed1() {
	setSpeed(1)
}

def speed2() {
	setSpeed(2)
}

def speed3() {
	setSpeed(3)
}

def speed4() {
	setSpeed(4)
}

def speed5() {
	setSpeed(5)
}

def speed6() {
	setSpeed(6)
}

def speed7() {
	setSpeed(7)
}

def speed8() {
	setSpeed(8)
}

def speedResume() {
	setSpeed(9)
}

def setSpeed(speed) {
	log.debug "Setting Speed to ${speed}"
    getSpeed(speed)
}

// Make a call to the Pool Controller
private getSpeed(command) {
    //Particle API Call
    def speedClosure = { response ->
	  	log.debug "Speed Request was successful, $response.data"
      
      	sendEvent(name: "currentSpeed", value: response.data.return_value)
	}
    
    def speedParams = [
  		uri: "https://api.particle.io/v1/devices/${deviceId}/mOverride",
        body: [access_token: token, command: command],  
        success: speedClosure
	]

	httpPost(speedParams)
}