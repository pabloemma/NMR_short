'''
Created on May 31, 2016

@author: klein
'''
import os
import subprocess
from imaplib import Commands

class MyRunNMR(object):
    '''
    This class will run the NMR_short program and converts all the xls files into root files
    It basically uses OS run commands.
    '''


    def __init__(self, directory):
        '''
        Constructor
        '''
        self.work_directory = directory
        
        #store current directory
        self.start_directory = os.getcwd()
        
        # go to working ditectory
        os.chdir(self.work_directory)
     
    def Run(self):
        '''
        execute the run commands, this simply calling the program and having the right arguments
        '''
        prog = "/Users/klein/git/NMR_short/ReadNMR_short/Debug/ReadNMR_short "
        arg0 = "-b"  # do it in batch
        arg01 = "batch"
        arg1 = "-i 3544230036"
        spac = " "
        arg2 = "-k"+self.work_directory
        tot_arg1 = prog +spac+arg0+spac+arg01+spac+arg1
        
        
        tot_arg=tot_arg1+spac+arg2
        print tot_arg
        
        subprocess.call("ls")
        os.system(tot_arg)
        print "finished"
        
    def Close(self):
        '''
        retunr to original state
        '''
        os.chdir(self.start_directory)
           
        


if __name__ == '__main__':
    a=MyRunNMR("//Volumes/FastDisk/NMR/rssmt_backup/april22/") # init system
    a.Run() # execute Commands
    a.Close()