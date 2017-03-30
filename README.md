Trunk Recorder v2.2.0 with Ncurses
=================
I'm going to leave the original instructions unchanged at the bottom of my additions.

This branch now incorporates extended colors in ncurses, and thus requires ncurses6, which can be obtained from here https://www.gnu.org/software/ncurses/
When you run './configure'you need to add the --enable-ext-colors option, it should look like this
```
./configure --enable-ext-colors
```

Make sure to uninstall older versions of ncurses first!

![Interface](images/9-6.PNG?raw=true )

### Config.json Changes
I've added some flags to the config file in order to control the default actions of ncurses.
with addition of the changes, the file should look like this at the end.
```
"captureDir": "$Capture_Directory",
"ncurses": 1,
"ncurses_group": 0,
"ncurses_cpu": 1,
"ncurses_dbg": 1,
"ncurses_lavg": 0
]
```

The options they control are as follows:
"ncurses": is the ncurses interface itself, 1 for enabled, 0 for disabled
"ncurses_group": is the default window assortment, 0 is all in the large format shown above (though rearranged as shown in pictures below), 1 is Recorders, Call Data, and Message Rate, 2 is Past Calls, CPU, and Missing (Missing talkgroups from the CSV files), and 3 is Errors and Logging in a smaller format. Pictures of all these are below.
"ncurses_cpu": This controls the default setting for having either individual CPU bars, or one average
"ncurses_dbg": This is not currently used but will eventually control debug/verbose logging on screen
"ncurses_lavg": This controls the default for having the system load average values (1, 5, 15 minute) along with the CPU window

I've also added different system tracking so I can keep things separate on the interface, so you have to add the hex system ID between the type and talkgroup file. 
This also includes conventional recorders, where each conventional system will overlap talkgroup numbers, it is best to give each a unique nac value.
If no nac is provided, then it will simply count how many systems there are.
```
"type": "p25",
"sysId": "2d0",
"sysSite": "2d3", This is used if you want to have multiple sites of a same system, it will allow you to record calls as one system, but display 2 names, if not provided, defaults to sysId
"talkgroupsFile": "$TG.csv",
```
or
```
"type": "smartnet",
"sysId": "f18",
"talkgroupsFile": "$TG.csv",
```

Another optional flag to go with the system settings is autoRetune, this sets whether or not it is allowed to use recorder's automatic retuning feature. I've added this because it was causing logging spam on the ncurses screen due to one system with poor signal.
To use it simply add 
```
"autoRetune": 0,
```
This will disable Retuning, the default value of 1 is assumed if you do not add it.

Another thing of note for conventional systems is that it can be extremely helpful if squelch is set, to use the 
```
"idleSilence": 1,
```
flag when defining a radio/source, this will help remove static on analog recorders.

### Keyboard Controls
'0': Sets Window Group 0
'1': Sets Window Group 1
'2': Sets Window Group 2
'3': Sets Window Group 3
'4': Sets Window Group 4 (currently blank)
'C'/'c': Changes the ncurses_cpu value
'V'/'v': Changes the ncurses_lavg value
'R': Sends a resized window trigger redrawing all the windows
'r': Does a "clear()" at the next refresh, should clear out any std::cout messages stuck on the screen

### Group 0
![Group 0](images/ncurses_big2.PNG)

### Group 1
![Group 1](images/ncurses_group1.PNG)

### Group 2
![Group 2](images/ncurses_group2.png)

### Group 3
![Group 3](images/ncurses_group3.PNG)

### Group 4
![Group 4](images/ncurses_group4.png)

### Group 5
![Group 5](images/ncurses_group5.png)

### Group 6
![Group 6](images/ncurses_group6.png)

### Group 7
![Group 7](images/ncurses_group7.png)

### Group 8
![Group 8](images/ncurses_group8.png)

### CPU Options
![CPU Options](images/cpu_options.png)

### Note: The format for the Config.json file changed in v2.x.

Need help? Got something working? Share it!


[![Chat](http://img.shields.io/badge/gitter-USER / REPO-blue.svg)]( https://gitter.im/trunk-recorder/Lobby?utm_source=share-link&utm_medium=link&utm_campaign=share-link) - [Google Groups](https://groups.google.com/d/forum/trunk-recorder) - and don't forget the [Wiki](https://github.com/robotastic/trunk-recorder/wiki)

Trunk Recorder is able to record the calls on trunked and conventional radio systems. It uses 1 or more Software Defined Radios (SDRs) to do this. The SDRs capture large swatches of RF and then use software to process what was received. GNURadio is used to do this processing because it provides lots of convenient RF blocks that can be pieced together to allow for complex RF processing. Multiple radio systems can be recorded at the same time.

Trunk Recorder currently supports the following:
 - Trunked P25 & SmartNet Systems
 - Conventional P25 & analog systems, where each group has a dedicated RF channel
 - SDRs that use the OsmoSDR source ( HackRF, RTL - TV Dongles, BladeRF, and more)
 - Ettus USRPs
 - P25 Phase 1, **P25 Phase 2** & Analog voice channels

I have tested things on both Unbuntu: 16.04, 14.04; OSX 10.10, OSX 10.11, 10.12. I have been using it with an Ettus b200, 3xRTL-SDR dongles and a HackRF Jawbreaker.

## Wiki Pages
### Installing
* [Docker](https://github.com/robotastic/trunk-recorder/wiki/Docker-Install)
* [Ubuntu 16.04](https://github.com/robotastic/trunk-recorder/wiki/Ubuntu-16.04-Install)
* [Raspberry Pi - Jessie](https://github.com/robotastic/trunk-recorder/wiki/Raspberry-Pi-Jessie-Install)

### Setup
* [Configuring a system](https://github.com/robotastic/trunk-recorder/wiki/Configuring-a-System)

###Playback & Sharing
By default, Trunk Recorder just dumps a lot of recorded files into a directory. Here are a couple of options to make it easier to browse through recordings and share them on the Internet.
* [OpenMHz](https://github.com/robotastic/trunk-recorder/wiki/Uploading-to-OpenMHz) - This is my free hosted platform for sharing recordings
* [Trunk Player](https://github.com/ScanOC/trunk-player) - A great Python based server, if you want to you want to run your own

* [FAQ](https://github.com/robotastic/trunk-recorder/wiki/FAQ)

___

##Install

###Requirements
 - GNURadio 3.7

**OSX**

If you are on OSX, the [MacPorts](https://gnuradio.org/redmine/projects/gnuradio/wiki/MacInstall) install of GNU Radio has worked for me.

###Setting up [GNU Radio](http://gnuradio.org/) on a fresh [Ubuntu](http://www.ubuntu.com/) Version 16.04 [Release](http://releases.ubuntu.com/16.04/)

There are a few methods to install GNU Radio. Source, [PyBOMBS](https://github.com/gnuradio/pybombs), or Distribution package manager. In this setup we will be using apt-get to install GNURadio (fastest method I have used). GNU Radio Version in apt-get as of 07/11/2016 is 3.7.9.  
Using a package manager is the currently preferred method from the GNU Radio Project, [Installing GNU Radio](http://gnuradio.org/redmine/projects/gnuradio/wiki/InstallingGR).

**Using apt-get to get GNU Radio and other prerequisites for Trunk Recorder**

Update currently install packages

`sudo apt-get update`  
`sudo apt-get upgrade`

Install GNU Radio with other prerequisites  
`sudo apt-get install gnuradio gr-osmosdr libhackrf-dev libuhd-dev`  

Install tools to compile Trunk Recorder  
`sudo apt-get install git cmake build-essential libboost-all-dev libusb-1.0-0.dev libssl-dev`  

Get source for Trunk Recorder  
Note: I put all my Radio related code into ~/radio/, change this as you wish  

`mkdir ~/radio`  
`cd ~/radio/`  
`git clone https://github.com/robotastic/trunk-recorder.git`  
`cd trunk-recorder`  
`cmake .`  
`make`  

**Running trunk recorder.**

If all goes well you should now have the executable named recorder.  
Before you can run anything, you need to create a `config.json` file ( see below ).
After you have done that, just run:  
`./recorder`

##Configure
Configuring Trunk Recorder and getting things setup can be rather complex. I am looking to make things simpler in the future.

**config.json**

This file is used to configure how Trunk Recorder is setup. It defines the SDRs that are available and the trunk system that will be recorded. Trunk Recorder will look for a *config.json* file in the same directory as it is being run in. You can point it to a different config file by using the *--config* argument on the command line, for example: `./recorder --config=examples/config-wmata-rtl.json`. The following is an example for my local system in DC, using an Ettus B200:

```
{
    "sources": [{
        "center": 857000000.0,
        "rate": 8000000.0,
        "squelch": -50,
        "error": 0,
        "gain": 40,
        "antenna": "TX/RX",
        "digitalRecorders": 2,
        "driver": "usrp",
        "device": "",
        "modulation": "qpsk"
    }],
    "systems": [{
        "control_channels": [855462500],
        "type": "p25",
        "talkgroupsFile": "ChanList.csv"
    }]
}
```
Here are the different arguments:
 - **sources** - an array of JSON objects that define the different SDRs available. The following options are used to configure each Source:
   - **center** - the center frequency in Hz to tune the SDR to
   - **rate** - the sampling rate to set the SDR to, in samples / second
   - **squelch** - Analog Squelch, my rtl-sdr's are around -60. [0 = Disabled] _Squelch needs to be set if the System using the source is conventional._
   - **error** - the tuning error for the SDR in Hz. This is the difference between the target value and the actual value. So if you wanted to recv 856MHz but you had to tune your SDR to 855MHz to actually receive it, you would set this to -1000000. You should also probably get a new SDR if it is off by this much.
   - **gain** - the RF gain to set the SDR to. Use a program like GQRX to find a good value.
   - **ifGain** - [hackrf only] sets the if gain.
   - **bbGain** - [hackrf only] sets the bb gain.
   - **mixGain** - [AirSpy only] sets the mix gain.
   - **lnaGain** - [AirSpy only] sets the lna gain.
   - **antenna** - [usrp] lets you select which antenna jack to user on devices that support it
   - **digitalRecorders** - the number of Digital Recorders to have attached to this source. This is essentially the number of simultaneous calls you can record at the same time in the frequency range that this Source will be tuned to. It is limited by the CPU power of the machine. Some experimentation might be needed to find the appropriate number.
   - **digitalLevels** - the amount of amplification that will be applied to the digital audio. The value should be between 1-16. The default value is 8.
   - **modulation** - the type of modulation that the system uses. The options are *qpsk* & *fsk4*. It is possible to have a mix of sources using fsk4 and qpsk demodulation.
   - **analogRecorders** - the number of Analog Recorder to have attached to this source. This is the same as Digital Recorders except for Analog Voice channels.
   - **analogLevels** - the amount of amplification that will be applied to the analog audio. The value should be between 1-32. The default value is 8.
   - **debugRecorders** - the number of Debug Recorder to have attached to this source. Debug Recorders capture a raw sample that you can examine later using GNURadio Companion. This is helpful if you want to fine tune your the error and gain for this Source.
   - **driver** - the GNURadio block you wish to use for the SDR. The options are *usrp* & *osmosdr*.
   - **device** - osmosdr device name and possibly serial number or index of the device, see [osmosdr page](http://sdr.osmocom.org/trac/wiki/GrOsmoSDR) for each device and parameters. You only need to do this if there are more than one. (bladerf=00001 for BladeRF with serial 00001 or rtl=00923838 for RTL-SDR with serial 00923838, just airspy for an airspy) It seems that when you have 5 or more RTLSDRs on one system you need to decrease the buffer size. I think it has something to do with the driver. Try adding buflen: "device": "rtl=serial_num,buflen=65536", there should be no space between buflen and the comma
 - **systems** - An array of JSON objects that define the trunking systems that will be recorded. The following options are used to configure each System.
   - **control_channels** - *(For trunked systems)* an array of the control channel frequencies for the system, in Hz. The frequencies will automatically be cycled through if the system moves to an alternate channel.
   - **channels** - *(For conventional systems)* an array of the channel frequencies, in Hz, used for the system. The channels get assigned a virtual talkgroup number based upon their position in the array. Squelch levels need to be specified for the Source(s) being used.
   - **type** - the type of trunking system. The options are *smartnet*, *p25*,  *conventional* & *conventionalP25*.
   - **talkgroupsFile** - this is a CSV file that provides information about the talkgroups. It determines whether a talkgroup is analog or digital, and what priority it should have. This file should be located in the same directory as the trunk-recorder executable.
   - **recordUnknown** - record talkgroups if they are not listed in the Talkgroups File. The options are *true* and *false* (without quotes). The default is *true*.
   - **shortName** - this is a nickname for the system. It is used to help name and organize the recordings from this system. It should be 4-6 letters with no spaces.
   - **uploadScript** - this script is called after each recording has finished. Checkout *encode-upload.sh.sample* as an example. The script should be located in the same directory as the trunk-recorder executable.
   - **audioArchive** - should the recorded audio files be kept after successfully uploading them. The options are *true* and *false* (without quotes). The default is *true*.
   - **callLog** - should a json file with the call details be generated. The options are *true* and *false* (without quotes). The default is *true*.
   - **bandplan** - [SmartNet only] this is the SmartNet bandplan that will be used. The options are *800_standard*, *800_reband*, *800_splinter*, and *400_custom*. *800_standard* is the default.
   - **bandplanBase** - [SmartNet, 400_custom only] this is for the *400_custom* bandplan only. This is the base frequency, specified in Hz.
   - **bandplanHigh** - [SmartNet, 400_custom only] this is the highest channel in the system, specified in Hz.
   - **bandplanSpacing** - [SmartNet, 400_custom only] this is the channel spacing, specified in Hz. Typically this is *25000*.
   - **bandplanOffset** - [SmartNet, 400_custom only] this is the offset used to calculate frequencies.
 - **defaultMode** - Default mode to use when a talkgroups is not listed in the **talkgroupsFile** the options are *digital* or *analog*.
 - **captureDir** - the complete path to the directory where recordings should be saved.
 - **callTimeout** - a Call will stop recording and save if it has not received anything on the control channel, after this many seconds. The default is 3.

**ChanList.csv**

This file provides info on the different talkgroups in a trunking system. A lot of this info can be found on the Radio Reference website. You need to be a site member to download the table for your system. If you are not, try clicking on the "List All in one table" link, selecting everything in the table and copying it into Excel or a spreadsheet.

You will have to add an additional column that adds a priority for each talkgroup. You need that number of recorders available to record a call at that priority. So, 1 is the highest, you would need 2 recorders available to record a priority 2, 3 record for a priority 3 and so on.

The Trunk Record program really only uses the priority information and the Dec Talkgroup ID. The Website uses the same file though to help display information about each talkgroup.

Here are the column headers and some sample data:

| DEC |	HEX |	Mode |	Alpha Tag	| Description	| Tag |	Group | Priority |
|-----|-----|------|-----------|-------------|-----|-------|----------|
|101	| 065	| D	| DCFD 01 Disp	| 01 Dispatch |	Fire Dispatch |	Fire | 1 |
|2227 |	8b3	| D	| DC StcarYard	| Streetcar Yard |	Transportation |	Services | 3 |

###Multiple SDR
Most trunk systems use a wide range of spectrum. Often a more powerful SDR is needed to have enough bandwidth to capture all of the potential channels that a system may broadcast on. However it is possible to use multiple SDRs working together to cover all of the channels. This means that you can use a bunch of cheap RTL-SDR to capture an entire system.

In addition to being able to use a cheaper SDR, it also helps with performance. When a single SDR is used, each of the Recorders gets fed all of the sampled signal. Each Recorder needs to cut down the multi-megasamples per second into a small 12.5Khz sliver. When you use multiple SDRs, each SDR is capturing only partial slice of the system so the Recorders have to cut down a much smaller amount of sample to get to the sliver they are interested in. This menans that you can have a lot more recorders running!

To use mutliple SDRs, simply define additional Sources in the Source array. The `confing-multi-rtl.json.sample` has an example of how to do this. In order to tell the different SDRs apart and make sure they get the right error correction value, give them a serial number using the `rtl_eeprom -s` command and then specifying that number in the `device` setting for that Source, `rtl=2`.

###How Trunking Works
Here is a little background on trunking radio systems, for those not familiar. In a Trunking system, one of the radio channels is set aside for to manage the assignment of radio channels to talkgroups. When someone wants to talk, they send a message on the control channel. The system then assigns them a channel and sends a Channel Grant message on the control channel. This lets the talker know what channel to transmit on and anyone who is a member of the talkgroup know that they should listen to that channel.

In order to follow all of the transmissions, this system constantly listens to and decodes the control channel. When a channel is granted to a talkgroup, the system creates a monitoring process. This process will start to process and decode the part of the radio spectrum for that channel which the SDR is already pulling in.

No message is transmitted on the control channel when a talkgroup’s conversation is over. So instead the monitoring process keeps track of transmissions and if there has been no activity for 5 seconds, it ends the recording.
