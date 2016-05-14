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
	}

	preferences {
		input("deviceId", "text", title: "Device ID")
		input("token", "text", title: "Access Token")
	}

	tiles(scale: 2) {
		valueTile("icon", "device.icon", width: 2, height: 2) {
			state("default", label:'${currentValue} RPM', icon:"st.Health & Wellness.health2", backgroundColor: "#7B8BFF")
		}
		valueTile("currentSpeed", "device.currentSpeed", width: 6, height: 2) {
			state("default", label:'Currently: ${currentValue} RPM', backgroundColor: "#7B8BFF")
		}
		standardTile("off", "device.off", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"off", label:'Set OFF', backgroundColor:"#F7C4BA")
			state("active", action:"off", label:'Set OFF', backgroundColor:"#F7C4BA")
		}
		standardTile("speed2", "device.speed2", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed2",  label:'Set Speed 2', backgroundColor:"#F7C4BA")
			state("active", action:"speed2", label:'Set Speed 2', backgroundColor:"#F7C4BA")
		}
		standardTile("speed3", "device.speed3", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed3", label:'Set Speed 3', backgroundColor:"#F7C4BA")
			state("active", action:"speed3", label:'Set Speed 3', backgroundColor:"#F7C4BA")
		}
		standardTile("speed4", "device.speed4", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed4", label:'Set Speed 4', backgroundColor:"#F7C4BA")
			state("active", action:"speed4", label:'Set Speed 4', backgroundColor:"#F7C4BA")
		}
		standardTile("speed5", "device.speed5", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed5", label:'Set Speed 5', backgroundColor:"#F7C4BA")
			state("active", action:"speed5", label:'Set Speed 5', backgroundColor:"#F7C4BA")
		}
		standardTile("speed6", "device.speed6", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed6",   label:'Set Speed 6', backgroundColor:"#F7C4BA")
			state("active", action:"speed6", label:'Set Speed 6', backgroundColor:"#F7C4BA")
		}
		standardTile("speed7", "device.speed7", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed7", label:'Set Speed 7', backgroundColor:"#F7C4BA")
			state("active", action:"speed7", label:'Set Speed 7', backgroundColor:"#F7C4BA")
		}
		standardTile("speed8", "device.speed8", decoration: "flat", width: 3, height: 1) {
			state("inactive", action:"speed8", label:'Set Speed 8', backgroundColor:"#F7C4BA")
			state("active", action:"speed8", label:'Set Speed 8', backgroundColor:"#F7C4BA")
		}
		standardTile("refresh", "device.refresh", decoration: "flat", width: 6, height: 1) {
			state("default", action:"refresh.refresh", label:"Refresh Current Speed", icon:"st.secondary.refresh", backgroundColor:"#F7C4BA")
		}

	main "icon"   
	details(["currentSpeed", "off", "speed2", "speed3", "speed4", "speed5", "speed6", "speed7", "speed8"])

	}
}

def parse(String description) { }

def poll() { updateDeviceData(parent.getDeviceData(this.device)) }

def refresh() { parent.refresh() }

def updateDeviceData(actualData) {
	def deviceData = convertTemperatureUnit(actualData, getTemperatureScale())
	sendEvent(name: "heatingSetpoint", value: deviceData.setPoint, unit: getTemperatureScale())
}

def off(){
	put '1'
}

def speed2() {
	put '2'
}

def speed3() {
	put '3'
}

def speed4() {
	put '4'
}

def speed5() {
	put '5'
}

def speed6() {
	put '5'
}

def speed7() {
	put '6'
}

def speed8() {
	put '7'
}

private put(overrideSpeed) {
    //Particle Photon API Call
    httpPost(
        uri: "https://api.particle.io/v1/devices/${deviceId}/overrideSpeed",
        body: [access_token: ${token}, command: overrideSpeed],  
    )
}