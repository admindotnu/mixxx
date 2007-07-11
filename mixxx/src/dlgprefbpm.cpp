/***************************************************************************
                          dlgprefmixer.cpp  -  description
                             -------------------
    begin                : Thu Jun 7 2007
    copyright            : (C) 2007 by John Sully
    email                : jsully@scs.ryerson.ca
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "dlgprefbpm.h"
#define MIXXX
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpushbutton.h>

#define CONFIG_KEY "[BPM]"


DlgPrefBPM::DlgPrefBPM(QWidget *parent, ConfigObject<ConfigValue> *_config) : DlgPrefBPMDlg(parent,"")
{
    config = _config;
    

    // Connection
    connect(chkAnalyzeEntireSong,   SIGNAL(stateChanged(int)), this, SLOT(slotSetAnalyzeMode()));
    connect(spinBoxBPMRangeStart,   SIGNAL(valueChanged(int)), this, SLOT(slotSetBPMRangeStart(int)));
    connect(spinBoxBPMRangeEnd,     SIGNAL(valueChanged(int)), this, SLOT(slotSetBPMRangeEnd(int)));
   
    // Set default value for analyze mode check box
    int iAnalyzeEntireSong = config->getValueString(ConfigKey("[BPM]","AnalyzeEntireSong")).toInt();
    if (iAnalyzeEntireSong)
        chkAnalyzeEntireSong->setChecked(true);
    else
        chkAnalyzeEntireSong->setChecked(false);
       

    //Load BPM Range Values
    int iRangeStart = config->getValueString(ConfigKey("[BPM]","BPMRangeStart")).toInt();
    if(iRangeStart > 0 && iRangeStart <= 220)
        spinBoxBPMRangeStart->setValue(iRangeStart);
    else
        spinBoxBPMRangeStart->setValue(60);
    
    int iRangeEnd = config->getValueString(ConfigKey("[BPM]","BPMRangeEnd")).toInt();
    if(iRangeEnd > 0 && iRangeEnd <=220)
        spinBoxBPMRangeEnd->setValue(iRangeEnd);
    else
        spinBoxBPMRangeEnd->setValue(180);

}

DlgPrefBPM::~DlgPrefBPM()
{
}

void DlgPrefBPM::slotSetAnalyzeMode()
{
    if (chkAnalyzeEntireSong->isChecked())
        config->set(ConfigKey("[BPM]","AnalyzeEntireSong"), ConfigValue(1));
    else
        config->set(ConfigKey("[BPM]","AnalyzeEntireSong"), ConfigValue(0));

}

void DlgPrefBPM::slotSetBPMRangeStart(int begin)
{
    config->set(ConfigKey("[BPM]","BPMRangeStart"),ConfigValue(begin));
}

void DlgPrefBPM::slotSetBPMRangeEnd(int end)
{
    config->set(ConfigKey("[BPM]","BPMRangeEnd"),ConfigValue(end));
}

void DlgPrefBPM::slotApply()
{
    int iRangeStart = config->getValueString(ConfigKey("[BPM]","BPMRangeStart")).toInt();
    int iRangeEnd = config->getValueString(ConfigKey("[BPM]","BPMRangeEnd")).toInt();

    if(iRangeStart > iRangeEnd)
    {
        if(iRangeStart >= 200)
        {
            iRangeEnd = 220;
        }
        else
        {
            iRangeEnd = iRangeStart + 20;
        }
    }

    spinBoxBPMRangeEnd->setValue(iRangeEnd);
}

void DlgPrefBPM::slotUpdate()
{
}

