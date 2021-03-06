#!bin/zsh
#Usage:
#	source runMode_Rerun.sh <Detector Name> <Data File Directory> <Config File - Analysis> <Config File - Mapping>

#store the original directory
DIR_ORIG=$PWD

#store the run config file
FILE_RUN=$GEM_BASE/config/configRun.cfg
FILE_RUN_TEMP=$GEM_BASE/config/configRun_Template_Rerun.cfg
cp $FILE_RUN_TEMP $FILE_RUN

#setup input variables
NAME_DET=$1
DIR_DATA=$2
FILE_ANA=$3
FILE_MAP=$4
#FILE_OUT=$5

#Move to the data directory
cd $DIR_DATA

#Replace Detector Name
sed -i -- "s@DETECTORNAME@$NAME_DET@g" $FILE_RUN

#Replace filenames
sed -i -- "s@CONFIGFILE_ANALYSIS@$FILE_ANA@g" $FILE_RUN
sed -i -- "s@CONFIGFILE_MAPPING@$FILE_MAP@g" $FILE_RUN
#sed -i -- "s@OUTPUTFILE@$FILE_OUT@g" $FILE_RUN

#Determine which files should be analyzed
sed -i '$ a [BEGIN_RUN_LIST]' $FILE_RUN
for f in *Ana.root
do
	INPUTFILENAME=$DIR_DATA/$f
	sed -i "$ a $INPUTFILENAME" $FILE_RUN
done
sed -i '$ a [END_RUN_LIST]' $FILE_RUN

#return to the original directory
cd $DIR_ORIG

#tell the user what to do
echo "I have created a run configuration file for you at:"
echo "$FILE_RUN"
echo ""
echo "To view this file execute:"
echo "gedit $FILE_RUN &"
echo ""
echo "To launch the analysis execute:"
echo "cd $GEM_BASE"
echo "./analyzeUniformity $FILE_RUN true"
