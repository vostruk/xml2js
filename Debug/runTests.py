import json
import os
import subprocess
programPath = os.getcwd()
program = "xml2js.exe"
filesPath = "Tests"
print("\n---- Stage 1 ----\nGenerating JSON files from XML files:")
for file in os.listdir(filesPath):
    if file.endswith(".xml"):
        subprocess.call([program, filesPath+"/"+file])
        print(file +".json generated")
print("\n----Stage 2 ----\nChecking if JSON files are valid:")
for file in os.listdir(filesPath):
    if file.endswith(".json"):
        with open(filesPath+'/'+file, 'r') as myfile:
            data=myfile.read()
            try:
                json.loads(data)
                print("JSON file "+file+" is OK")
            except ValueError as e:
                print('invalid json file '+file+ ", error: %s" %e)
input("Press Enter to continue...")
