<!-- The block below is configuration for the "pandoc" utility, which
allows generating a PDF of this file:
   pandoc -o name.pdf name.md
-->
---
title: Internet of Things — RFID Attendance System
subtitle: GE 100 Final Project
date: Fall 2017
author:
- Stewart Thomas
- Dan White
---


## Abstract
>   Build and demonstrate a system that records attendance by reading RFID
>   tags. The system will consist of:\
>   a) a WiFi-connected microcontroller and RFID reader,\
>   b) several RFID tags,\
>   c) an online database to record tag readings,\
>   d) a program to use records in the database to compute student
>   attendance.\
>   This project will require you to integrate embedded microprocessors
>   (Arduino), an RFID reader system, and Internet access.



# Project Goals

Successful completion of this project will include the following
milestones:

- Connect a WiFi-connected micro-controller (uC) to the Valpo wireless network.

- Post data from the uC to the web service [ThingSpeak](https://thingspeak.com).

- Read the unique serial number (EPC) from an RFID tag using an RFID reader.

- Post the EPC to a specific [ThingSpeak](https://thingspeak.com) channel.

- Use Matlab in the cloud to read the database and compute a daily attendance report.


# Details
=======

![RFID Tags](./fig/RFIDTags.jpg "RFID Tags")

RFID stands for “Radio-frequency Identification.”
Much like a barcode, this technology allows for items or things to be identified.
However, the system is entirely electronic and works without the tags requiring a battery to be identified.
You will combine several existing items together to create a system that wirelessly takes attendance and records the data on the Internet.
You will be using an RFID reader from ThingMagic, a Sparkfun development board, an Arduino microcontroller, and an online database from Mathworks.


## RFID Reader

You will use the <span style="font-variant:small-caps;">SparkFun
Simultaneous RFID Reader - M6E Nano</span> from Sparkfun. The website is
<https://www.sparkfun.com/products/14066>. Sparkfun provides many
libraries that will enable you to quickly get started.


## Arduino microcontroller

You will use the <span style="font-variant:small-caps;">SparkFun WiFi
Shield - ESP8266</span> available from
<https://www.sparkfun.com/products/13287>. This board features the
ESP8266, which has both a microcontroller and a WiFi radio on the same
chip. You will use the Arduino programming environment to a) interface
the RFID reader and get data from tags b) upload the data received from
the tag to the Internet using WiFi.

If you wish to use a different computer for programming, you will need
to install some extra board definitions and libraries in your Arduino
IDE. This guide was prepared with the Arduino IDE version 1.8.5.

-   Install the Arduino core for the ESP8266 by following the
    instructions at
    <https://github.com/esp8266/Arduino#installing-with-boards-manager>.

-   Select board: “Sparkfun ESP8266 Thing” from the Tools $\rightarrow$
    Board menu.

-   Install the **SparkFun Simultaneous RFID Tag Reader Library** using
    the Arduino Library Manager. Open this from the menu item: Sketch
    $\rightarrow$ Include Library $\rightarrow$ Manage Libraries. Then
    search for the library by name.

The documentation for the ESP8266 Arduino core is found at:\
<https://arduino-esp8266.readthedocs.io/en/latest/index.html>


## Mathworks Thingspeak

Mathworks (makers of MATLAB), provide an online service for ‘things’ to
store data they collect. You will create a new Thingspeak channel and
use the provided protocol, or Application Programming Interface (API) to
push your data to the Thingspeak channel. (Don’t worry, it sounds scary,
but it’s surprisingly easy.)


## Attendance Interface

Lastly, you will use a computer program such as MATLAB to download the
data from Thingspeak that contains dates and times of tags read, and
compare those to a list of students. From that information, you will
keep track of who is in attendance and report this.


# Workspace

You will be able to work in Gellersen 166, the Electronics Lab
(a.k.a. “Junior Lab”).


# Suggested Timeline

-   **Day 1 (Friday)** — Instructor briefing; Connect the RFID reader to
    the Arduino Thing Dev and read a set of tag ‘TID’s; Create a
    Thingspeak channel. (Find ThingDev MAC address and give
    to professors).

-   **Day 2 (Monday)** — Practice the Thingspeak API; Connect ThingDev
    to the Valparaiso WiFi network and upload data to the Thingspeak
    channel; Begin filtering data

-   **Day 3 (Tuesday)** — Combine RFID reader program and Thingspeak
    WiFi program; Begin uploading tag data to Thingspeak; Begin MATLAB
    program to pull down data from Thingspeak.

-   **Day 4 (Wednesday)** — Let MATLAB program filter data by time or
    date; Display tag id’s in attendance; Compare tag id’s to a list of
    students and print out names of students in attendance and names of
    students absent.

-   **Day 5 (Thursday)** — In-class presentation!

Note: Final Demonstration by 5:00 PM on Friday, November 3rd!
