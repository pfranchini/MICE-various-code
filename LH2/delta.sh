# Momentum loss vs run number during the LH2 filling

rm run_recon.root

for i in 9730 9736 9739 9663 9665 9666 9671 9676 9680 9681 9684 9688 9692 9695 9699 9724 9727; do    #6-140
#for i in 9654 9659 9660 9661 9677 9678 9679 9683 9687 9690 9691 9694 9698 9723 9726; do             #3-140


    run=$i
    echo "RUN: " $run
    
    if [ ! -f 0${run}_recon.root ]; then
	
	wget http://reco.mice.rl.ac.uk/MAUS-v3.0.0/0${run}_offline.tar
	
	if [ -f 0${run}_offline.tar ]
	then
	    
	    tar xf 0${run}_offline.tar
	    rm 0${run}_offline.tar

	fi
	
    fi
    
    
    
    if [ -f 0${run}_recon.root ]; then
	
	ls -l 0${run}_recon.root
	rm -f 0${run}_offline.tar
	rm -f 0${run}_reco.log
	
	ln -s 0${run}_recon.root run_recon.root
	
	root -q -l abs_momentum_loss.c

	rm run_recon.root

	echo "---"
	
    fi
    
done