import json
import os
import subprocess
program = "xml2js.exe"
for file in os.listdir("Tests"):
    if file.endswith(".xml"):
        subprocess.call([program, file])
		print(file)