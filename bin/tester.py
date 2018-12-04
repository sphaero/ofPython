#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
import glm
#print(dir(glm))
from openframeworks import *

def draw():
    color = ofColor( random.randint(0,255), random.randint(0,255), random.randint(0,255) )
    ofSetColor(color)
    ofDrawCircle( ofGetWidth()/2, ofGetHeight()/2, 100 )
    return 0

def test(a,b):
    return a + b

count = 0
def countuntil(a):
    count+=1
    return count%a

def test2():
    global count
    count+=1
    return count

