
# LA NAV

[![N|Solid](http://content.sportslogos.net/logos/35/882/thumbs/88247800.gif)](http://web.cs.ucla.edu/classes/winter17/cs32/)
**Find the shortest path between two attractions in the Westwood, West Los Angeles, West Hollywood, Brentwood, and Santa Monica areas!**

*Disclaimer: I am not responsible if you actually follow directions outputted by this program and cause mass chaos throughout LA. Use Google Maps if you really need to*

This program was developed as a final project for CS32 during the Winter Quarter of 2017 at UCLA. 

### Usage
*Note: make file coming soon*
clone this repo somewhere on your local machine
```sh
$ make
$ ./LaNav "{{location1}}" "{{location2}}"
```
Replace {{location1}} and {{location2}} with the two locations you want directions for

### Available Locations
All available locations can be found in
```
validlocs.txt
```

### Map Data
The map data used for this project was generously provided by [Carey Nachenberg](careynachenberg.weebly.com)

It uses
  - http://www.latlong.net to look up latitude and longitude for any address
  - http://wiki.openstreetmap.org/wiki/Downloading_data#Choose_your_region
  
### Contributions
I am open to any and all suggestions for improvement! If you or a loved-one was tragically injured while driving and following directions outputted by this program, and you feel it is your duty  to improve the quality of this program's navigation, fork this repo and have at it.
  
