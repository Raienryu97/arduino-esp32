#include <WiFi.h>

unsigned int request_i = 0;
unsigned int response_i = 0;

String manageRequest(String request);

/*
Get the base mac adress of the ESP32 and return it in a string.
*/
String getChipId(){
  esp_err_t status;
  uint64_t macPointer;
  int i;
  status = esp_efuse_mac_get_default((uint8_t*)(&macPointer));
  if(status == ESP_OK){
    char macRaw[1024];
    sprintf(macRaw,"%04X%08X",(uint16_t)(macPointer>>32),(uint32_t)macPointer);
    char macAddr[strlen(macRaw)];
    for(i=0;i<strlen(macRaw);i++){
      if(i%2 != 0){
        macAddr[i] = macRaw[strlen(macRaw)-i];
      }
      else{
        macAddr[i] = macRaw[strlen(macRaw)-2-i];
      }
    }
    macAddr[i] = '\0';
    return String(macAddr);
  }
  else{
    String err = "Unable to fetch Chip ID. BLK0 of EFUSE may be corrupted";
    return err;
  }
}

/* Create the mesh node object */
WiFiMesh mesh_node = WiFiMesh(getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String request)
{
	/* Print out received message */
	Serial.print("received: ");
	Serial.println(request);

	/* return a string to send back */
	//char response[60];
	//sprintf(response, "Hello world response #%d from Mesh_Node%d.", response_i++, getChipId());
	String response = "Hello world response #" + String(response_i++) + " from Mesh_Node" + getChipId() + ".";
	return response;
}

void setup()
{
	Serial.begin(115200);
	delay(10);

	Serial.println();
	Serial.println();
	Serial.println("Setting up mesh node...");

	/* Initialise the mesh node */
	mesh_node.begin();
}

void loop()
{
	/* Accept any incoming connections */
	mesh_node.acceptRequest();

	/* Scan for other nodes and send them a message */
	//char request[60];
	//sprintf(request, "Hello world request #%d from Mesh_Node%d.", request_i++, getChipId());
	String request = "Hello world request #" + String(request_i++) + " from Mesh_Node" + getChipId() + ".";
	mesh_node.attemptScan(request);
	delay(1000);
}
