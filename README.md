# Parking Sensor Realisation

## Goal of the project

With the development of the system, it is planned to implement a functional model that alerts the driver in real time to obstacles near the car. The system must be able to visually and audibly load inputs from sensors, process data and present them to the driver in real time with appropriate audible warnings.

## Working

**1st step:** Only Green is shown in any one corner or all the corners as a warning that the obstacle is near but is safe as shown in figure below.

![image](https://user-images.githubusercontent.com/77145863/178687670-72471b36-9cd7-4776-b69b-8520f81f6436.png)

**2nd Step:** If a car moves further to the direction of the obstacle another warning level is shown where the Green and Yellow is shown as a warning and the risk chance is 50% as shown in figure below.

![image](https://user-images.githubusercontent.com/77145863/178687806-49c18bfd-b217-49b3-95fe-b1a10c1e812e.png)

**3rd Step:** If a car moves further to the direction of the obstacle another warning level is shown where the Green, Yellow and Red is shown as a warning and the risk is every high and it is advisable to the driver not to go further from this position while parking as shown in figure below.

![image](https://user-images.githubusercontent.com/77145863/178687853-a72b9fbb-afb3-4189-b7a3-b5566f5cfade.png)

The sound is also played as a warning to the driver along with the above visual outputs. The 6 different sounds are used and played in the left or right or both channels based on the side where the obstacle is encountered. For the 1st step warning, the sound with high delay is used, for 2nd step warning, the sound with the medium delay is used. And for the 3rd step warning the sound is played continuously in small time interval so that the driver can be warned and it is advisable to the driver not to go further from this position while parking.
If there are two sensors reading the same value, the direction of drive is used to play the sound. An additional two generated sounds are played in situations where both front sensors are dominant hazard levels and read the same values, or both rear sensors are dominant hazard levels and read the same values. These two additional sounds are played on both channels.

## Development Environments required 

Softwares like Visual Studio, Eclipse or similar development environment supporting the C++ programming and the use of the libraries such as Open GL, Sound libraries and so on. The testing is done in the personal computer with the necessary hardware for the sound.

## Simulation

**Keyboard Keys Used for real-time simulation**
|Warning Sign |Front Left (mode 0)|Front Right (mode 1)|Rear left (mode 2)|Rear Right (mode 3)|
|-----------|-------------|-----------|-------------|-------------|
|**Green Only**|q|a|t|f|
|**Green and Yellow**|w|s|y|f|
|**Green Yellow and Red**|e|d|u|h|

**Additional Keys for Drive Mode**
|       | Front Drive Mode | Rear Drive Mode |
|-----------|-----------|-----------|
| **Drive Mode** | 1 | 2 |



