#!/bin/bash
date

#. ./cleanOutPut.sh

if [ $# -eq 0 ]
then
  Energy=200GeV_2014
  Mode=RunQA
  JobId=6DB5839CEBAADD58FC449474F2D751AD #generate faild list for this Job

  FileDirectory="/star/u/sunxuhit/AuAu$Energy/SpinAlignment/$Mode"

  OutPutDir="/star/u/sunxuhit/WorkSpace/VecMesonSpinAlignment_BESII/FileList/$Energy"
  FailedList="$OutPutDir/condor_failed_${Mode}_${JobId}.list"
  echo "delete following files from "
  echo $FailedList
  for item in `cat $FailedList`
  do
    echo deleting $FileDirectory/$item
    ls $FileDirectory/$item
    rm $FileDirectory/$item
  done
  # rm $FailedList

else
  echo "Wrong number of parameters"
fi