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
		capability "Switch"
		capability "Polling"
		capability "Refresh"

		// tile definitions.  must also define a parse method in your device handler with the same name
		command "off"
		command "speed2"
		command "speed3"
		command "speed4"
		command "speed5"
		command "speed6"
		command "speed7"
		command "speed8"
		command "updateData", ["string"]
		command "resume"
	}

	preferences {
		input("deviceId", "text", title: "Device ID", description: "Particle Device ID", required: true, displayDuringSetup: true)
		input("token", "text", title: "Access Token", description: "Particle User Access Token", required: true, displayDuringSetup: true)
	}

	tiles(scale: 2) {
		valueTile("icon", "device.icon", width: 2, height: 2) {
			state("default", label:'${currentValue} RPM', icon:"st.Health & Wellness.health2", backgroundColor: "#7B8BFF")
		}
		valueTile("currentSpeed", "device.currentSpeed", width: 3, height: 2) {
			state("default", label:'Currently: ${currentValue} RPM', backgroundColor: "#7B8BFF")
		}
		standardTile("off", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"setSpeed(1)", label:'Set OFF', nextState:"active", backgroundColor:"#F7C4BA")
			state("active", action:"setSpeed(1)", label:'Set OFF', backgroundColor:"#F7C4BA")
		}
		standardTile("speed2", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"setSpeed(2)",  label:'Set Speed 2', backgroundColor:"#F7C4BA")
			state("active", action:"setSpeed(2)", label:'Set Speed 2', backgroundColor:"#F7C4BA")
		}
		standardTile("speed3", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"setSpeed(3)", label:'Set Speed 3', backgroundColor:"#F7C4BA")
			state("active", action:"setSpeed(3)", label:'Set Speed 3', backgroundColor:"#F7C4BA")
		}
		standardTile("speed4", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed4", label:'Set Speed 4', backgroundColor:"#F7C4BA")
			state("active", action:"speed4", label:'Set Speed 4', backgroundColor:"#F7C4BA")
		}
		standardTile("speed5", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed5", label:'Set Speed 5', backgroundColor:"#F7C4BA")
			state("active", action:"speed5", label:'Set Speed 5', backgroundColor:"#F7C4BA")
		}
		standardTile("speed6", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed6",   label:'Set Speed 6', backgroundColor:"#F7C4BA")
			state("active", action:"speed6", label:'Set Speed 6', backgroundColor:"#F7C4BA")
		}
		standardTile("speed7", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed7", label:'Set Speed 7', backgroundColor:"#F7C4BA")
			state("active", action:"speed7", label:'Set Speed 7', backgroundColor:"#F7C4BA")
		}
		standardTile("speed8", "device.setSpeed", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed8", label:'Set Speed 8', backgroundColor:"#F7C4BA")
			state("active", action:"speed8", label:'Set Speed 8', backgroundColor:"#F7C4BA")
		}
		standardTile("refresh", "device.refresh", decoration: "flat", width: 3, height: 1) {
			state("default", action:"refresh", label:"Refresh Current Speed", icon:"st.secondary.refresh", backgroundColor:"#F7C4BA")
		}
		standardTile("resume", "device.resume", decoration: "flat", width: 3, height: 1) {
			state("default", action:"resume", label:"Resume Schedule", backgroundColor:"#F7C4BA")
		}

		main "icon"   
		details([
		"currentSpeed",	"refresh",
								"resume",
		"off",					"speed2",
		"speed3", 			"speed4",
		"speed5", 			"speed6",
		"speed7", 			"speed8"
		])
	}
}

// example
def parse(String description) {
	log.debug "parse description: $description"
}
/*
def poll() {
	updateDeviceData(parent.getDeviceData(this.device)) }

def refresh() {
	log.debug "Manually requested refresh"
	parent.refresh() }

def updateDeviceData(actualData) {
	def deviceData = convertTemperatureUnit(actualData, getTemperatureScale())
	sendEvent(name: "heatingSetpoint", value: deviceData.setPoint, unit: getTemperatureScale())
}*/

def setSpeed(x){
	put ${x}
}

def off(){
	log.debug "Set State: OFF"
	put '1'
}

def speed2() {
	log.debug "Set State: Speed 2"
	put '2'
}

def speed3() {
	log.debug "Set State: Speed 3"
	put '3'
}

def speed4() {
	log.debug "Set State: Speed 4"
	put '4'
}

def speed5() {
	log.debug "Set State: Speed 5"
	put '5'
}

def speed6() {
	log.debug "Set State: Speed 6"
	put '6'
}

def speed7() {
	log.debug "Set State: Speed 7"
	put '7'
}

def speed8() {
	log.debug "Set State: Speed 8"
	put '8'
}

def resume(){
	log.debug "Set State: Resume Schedule"
	put 'resume'
}

private put(newSpeed) {
    //Particle Photon API Call
    httpPost(
        uri: "https://api.particle.io/v1/devices/${deviceId}/newSpeed",
        body: [access_token: ${token}, command: newSpeed],  
    )
}