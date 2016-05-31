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
        arg10 = "-i" 
        filename = "3544230036"
        spac = " "
        arg2 = "-k"+self.work_directory
        # here we start the loop
        # we will need to strip NMR and .xls from the name
        tot_arg1 = prog +spac+arg0+spac+arg01+spac

        for file1 in self.file_list:
            file2 =file1.strip("NMR")
            file3 = file2.strip(".xls")
            tot_arg=tot_arg1+spac+arg10+spac+file3+spac+arg2
           
        
        
            print tot_arg
        
        #subprocess.call("ls")
            os.system(tot_arg)
        print "finished"
        
    
    def CreateFileList(self):
        ''' creates a list of files to be used
        for conversion
        '''
        self.file_list = []
        for file1  in os.listdir(self.work_directory):
            if file1.count(".xls"):
                if not file1.count("ave"):
                    self.file_list.append(file1)
                    #print file1
                
        
    
    
    def Close(self):
        '''
        retunr to original state
        '''
        os.chdir(self.start_directory)
           
        


if __name__ == '__main__':
    a=MyRunNMR("//Volumes/FastDisk/NMR/rssmt_backup/april22/") # init system
    a.CreateFileList()
    a.Run() # execute Commands
    a.Close()