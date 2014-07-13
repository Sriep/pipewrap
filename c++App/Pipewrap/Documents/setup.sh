#!/bin/sh
qhelpgenerator pipewrap.qhp -o pipewrap.qch
assistant -register pipewrap.qch
qcollectiongenerator pipewrap.qhcp -o pipewrap.qhc
assistant -collectionFile pipewrap.qhc
