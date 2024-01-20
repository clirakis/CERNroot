/**
 ******************************************************************
 *
 * Module Name : hdf5ToRoot.hh
 *
 * Author/Date : C.B. Lirakis / 22-Feb-22
 *
 * Description : Template for a main class
 *
 * Restrictions/Limitations : none
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *
 *******************************************************************
 */
#ifndef __HDF5TOROOT_hh_
#define __HDF5TOROOT_hh_
#  include "TROOT.h"
#  include "CObject.hh" // Base class with all kinds of intermediate
#  include "H5Logger.hh"
#  include "filename.hh"
class TNtupleD;

class hdf5ToRoot : public CObject
{
public:
    /** 
     * Build on CObject error codes. 
     */
    enum {ENO_FILE=1, ECONFIG_READ_FAIL, ECONFIG_WRITE_FAIL};
    /**
     * Constructor the hdf5ToRoot subsystem.
     * All inputs are in configuration file. 
     */
    hdf5ToRoot(const char *ConfigFile, const char *Filename);

    /**
     * Destructor for SK8. 
     */
    ~hdf5ToRoot(void);

    /*! Access the This pointer. */
    static hdf5ToRoot* GetThis(void) {return fhdf5ToRoot;};

    /**
     * Main Module DO
     * 
     */
    void Do(void);

    /**
     * Tell the program to stop. 
     */
    void Stop(void) {fRun=false;};

    /**
     * Control bits - control verbosity of output
     */
    static const unsigned int kVerboseBasic    = 0x0001;
    static const unsigned int kVerboseMSG      = 0x0002;
    static const unsigned int kVerboseFrame    = 0x0010;
    static const unsigned int kVerbosePosition = 0x0020;
    static const unsigned int kVerboseHexDump  = 0x0040;
    static const unsigned int kVerboseCharDump = 0x0080;
    static const unsigned int kVerboseMax      = 0x8000;
 
private:

    bool fRun;

    /*!
     * ingest data from HDF5 file.  
     */
    H5Logger    *f5InputFile;

    /*! 
     * Output data to NTUPLE
     */
    TFile       *fRootFile;
    TNtupleD    *fNtuple;        // Output

    /*! 
     * Configuration file name. 
     */
    char   *fConfigFileName;

    /* Collection of configuration parameters. */
    bool   fLogging;       /*! Turn logging on. */


    /* Private functions. ==============================  */

    /*!
     * Open the data HDF5 input file. 
     */
    bool OpenInputFile(const char *Filename);

    /*!
     * Create Root file/ntuple for output. 
     */
    bool OpenOutputFile(const char *Filename);

    /*!
     * Read the configuration file. 
     */
    bool ReadConfiguration(void);
    /*!
     * Write the configuration file. 
     */
    bool WriteConfiguration(void);


    /*! The static 'this' pointer. */
    static hdf5ToRoot *fhdf5ToRoot;
};
#endif
