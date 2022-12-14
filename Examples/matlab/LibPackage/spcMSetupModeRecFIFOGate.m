%**************************************************************************
% Spectrum Matlab Library Package               (c) Spectrum GmbH, 2018
%**************************************************************************
% Supplies different common functions for Matlab programs accessing the 
% SpcM driver interface. Feel free to use this source for own projects and
% modify it in any kind
%**************************************************************************
% spcMSetupModeRecFIFOGate:
% record FIFO mode gated sampling
%**************************************************************************

function [success, cardInfo] = spcMSetupModeRecFIFOGate (cardInfo, chEnableH, chEnableL, preSamples, postSamples, gatesToRec)

	global mRegs;
    if (isempty (mRegs))
        mRegs = spcMCreateRegMap ();
    end

    error = 0;
    
    % ----- setup the mode -----
    error = error + spcm_dwSetParam_i32 (cardInfo.hDrv, mRegs('SPC_CARDMODE'), mRegs('SPC_REC_FIFO_GATE'));
    error = error + spcm_dwSetParam_i64m (cardInfo.hDrv, mRegs('SPC_CHENABLE'), chEnableH, chEnableL);
    error = error + spcm_dwSetParam_i64 (cardInfo.hDrv, mRegs('SPC_PRETRIGGER'), preSamples);
    error = error + spcm_dwSetParam_i64 (cardInfo.hDrv, mRegs('SPC_POSTTRIGGER'), postSamples);
    error = error + spcm_dwSetParam_i64 (cardInfo.hDrv, mRegs('SPC_LOOPS'), gatesToRec);
    
    % ----- store some information in the structure -----
    cardInfo.setMemsize      = 0;
    cardInfo.setChEnableHMap = chEnableH;
    cardInfo.setChEnableLMap = chEnableL;
    
    [errorCode, cardInfo.setChannels] = spcm_dwGetParam_i32 (cardInfo.hDrv, mRegs('SPC_CHCOUNT'));
    error = error + errorCode;
    
    [success, cardInfo] = spcMCheckSetError (error, cardInfo);
    
    
    
    
    

    