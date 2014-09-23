#!/bin/bash

udevadm info --name=$1 --attribute-walk
