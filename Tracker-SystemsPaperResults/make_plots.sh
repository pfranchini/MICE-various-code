
LOCAL_DIR=/home/hep/ch308/work/My_MAUS/SystemsPaperResults
SCRIPT="/home/hep/ch308/work/My_MAUS/make_plots.py"



mkdir -p $LOCAL_DIR/plots/resolutions/09952 $LOCAL_DIR/plots/resolutions/09959 $LOCAL_DIR/plots/resolutions/09964



#python $SCRIPT resolution -D $LOCAL_DIR/plots/resolutions/09952 data/resolutions/resolutions_09952.root
#
#python $SCRIPT resolution -D $LOCAL_DIR/plots/resolutions/09959 data/resolutions/resolutions_09959.root
#
#python $SCRIPT resolution -D $LOCAL_DIR/plots/resolutions/09964 data/resolutions/resolutions_09964.root


python $SCRIPT efficiency -D $LOCAL_DIR/plots/efficiencies/H25c data/efficiencies/H25c.root

python $SCRIPT efficiency -D $LOCAL_DIR/plots/efficiencies/H36aa data/efficiencies/H36aa.root

python $SCRIPT efficiency -D $LOCAL_DIR/plots/efficiencies/H36c data/efficiencies/H36c.root

python $SCRIPT efficiency -D $LOCAL_DIR/plots/efficiencies/H36d data/efficiencies/H36d.root

