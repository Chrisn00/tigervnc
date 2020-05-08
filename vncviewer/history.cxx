/* Copyright (C) 2020 CKN
 */

#include <stdio.h>
#include <os/os.h>
#include <rfb/Exception.h>
//#include <rfb/LogWriter.h>

#include <FL/fl_utf8.h>

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "i18n.h"
using namespace rfb;
#include "history.h"

std::vector<std::string> LoadHistory(void){
	char* homeDir = NULL;
	char filepath[PATH_MAX];
  if (getvnchomedir(&homeDir) == -1)
    throw Exception(_("Failed to read configuration file, "
                      "can't obtain home directory path."));

  snprintf(filepath, sizeof(filepath), "%shistory", homeDir);
  delete[] homeDir;
	return LoadHistory(filepath);
}

void SaveHistory(std::vector<std::string> History){
	char* homeDir = NULL;
	char filepath[PATH_MAX];
  if (getvnchomedir(&homeDir) == -1)
    throw Exception(_("Failed to read configuration file, "
                      "can't obtain home directory path."));

  snprintf(filepath, sizeof(filepath), "%shistory", homeDir);
  delete[] homeDir;
	SaveHistory(filepath,History);
}

std::vector<std::string> LoadHistory(const char *filename){
	const size_t buffersize = 256;
	std::vector<std::string> History;
	char line[buffersize];
	
	/* Open history file */
  FILE* f = fopen(filename, "r");
  if (!f) {
    return History; // Use defaults.
  }
	int lineNr = 0;
  while (!feof(f)) {

    // Read the next line
    lineNr++;
    if (!fgets(line, sizeof(line), f)) {
      if (feof(f))
        break;

      throw Exception(_("History: Failed to read line %d in file %s: %s"),
                      lineNr, filename, strerror(errno));
    }

		// Fail if line too lopng
    if (strlen(line) == (sizeof(line) - 1))
      throw Exception(_("History: Failed to read line %d in file %s: %s"),
                      lineNr, filename, _("Line too long"));
                      
    // Skip commented line
		if ((line[0] == '\n') || (line[0] == '#') || (line[0] == '\r'))
      continue;
      
    // Remove new lines
    int len = strlen(line);
    if (line[len-1] == '\n') {
      line[len-1] = '\0';
      len--;
    }
    
    // Skip empty line
    if(line[0]=='\0')
    	continue;
    	
    // Add history value
    History.push_back(std::string(line));
  }
  fclose(f);
  // Return VNC history
  return History;
}

void SaveHistory(const char* filename,std::vector<std::string> History){
	/* Open history file */
  FILE* f = fopen(filename, "w");
  if (!f) {
    throw Exception(_("History: Failed to write file %s: %s"),
                      filename, strerror(errno));
  }
  
  // Write all history to file
	for(std::size_t i=0; i<History.size(); ++i){
		fprintf(f, "%s\n", History[i].c_str());
	}
	fclose(f);
}



