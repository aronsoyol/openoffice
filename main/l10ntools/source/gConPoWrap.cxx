/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/
using namespace std;
#include "gConPo.hxx"



/*****************************************************************************
 **********************   G C O N P O W R A P . C X X   **********************
 *****************************************************************************
 * This includes the c code generated by flex
 *****************************************************************************/



/**********************   I M P L E M E N T A T I O N   **********************/
#include "gConPo_yy.c"



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_po_impl::runLex()
{
  genpo_lex();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_po_impl::pushKeyPart(TAG_TYPE bIsNode, string& sTag)
{
  // remember text for merge
  msCollector += sTag;

  po_stack_entry newTag(bIsNode, sTag);

  mcStack.push(newTag);
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_po_impl::popKeyPart(TAG_TYPE bIsNode, string &sTag)
{
  // remember text for merge
  msCollector += sTag;

  // check for correct node/prop relations
  if (bIsNode != mcStack.top().mbIsNode)
    throw "node/prob mismatch in file " + msSourceFile;

  mcStack.pop();

  if (mbMergeMode && msCollector.size())
    writeSourceFile(msCollector);
  msCollector.clear();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_po_impl::startCollectData(string& sCollectedText)
{
  if (mbMergeMode && msCollector.size())
    writeSourceFile(msCollector);

  mbCollectingData = true;
  msCollector.clear();
}



/**********************   I M P L E M E N T A T I O N   **********************/
void convert_po_impl::stopCollectData(string& sCollectedText)
{
  string useKey;


  // locate key and extract it
  while (mcStack.size())
  {
    po_stack_entry nowEntry = mcStack.top();
	mcStack.pop();
	useKey = useKey + nowEntry.msName;
  }

  if (mbMergeMode)
  {
    // get all languages (includes en-US)
    vector<l10nMem_entry *>& cExtraLangauges = mcMemory.getLanguagesForKey(useKey);
    string                   sNewLine;
    int                      nL = cExtraLangauges.size();

	writeSourceFile(msCollector + sCollectedText);
    for (int i = 0; i < nL; ++i)
    {
      sNewLine = "<value xml:lang=\"" + cExtraLangauges[i]->msLanguage + "\">" +
		         cExtraLangauges[i]->msText + "</value>";
      writeSourceFile(sNewLine);
    }
  }
  else
    mcMemory.setEnUsKey(useKey, sCollectedText);

  mbCollectingData = false;
  msCollector.clear();
}  


/**********************   I M P L E M E N T A T I O N   **********************/
void convert_po_impl::collectData(string& sCollectedText)
{
  msCollector += sCollectedText;
  if (sCollectedText == "\n")
  {
    if (mbMergeMode)
      writeSourceFile(msCollector);
    msCollector.clear();
  }
}