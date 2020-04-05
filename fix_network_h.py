
from tempfile import mkstemp
from shutil import move, copymode, rmtree
import os
import re


pat1 = re.compile("#define AI_NETWORK_IN { \\\\")
pat2 = re.compile("#define AI_NETWORK_IN_SIZE { \\\\")
pat3 = re.compile("#define AI_NETWORK_OUT { \\\\")
pat4 = re.compile("#define AI_NETWORK_OUT_SIZE { \\\\")
pat5 = re.compile("\\\\")
pat6 = re.compile("}")

cor_in_line1 = "#define AI_NETWORK_IN AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_FLOAT, 30, 1, 12, 1, NULL)\n"
cor_in_line2 = "#define AI_NETWORK_IN_SIZE (30 * 1 * 12)\n"
cor_in_line3 = "#define AI_NETWORK_OUT AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_FLOAT, 1, 1, 7, 1, NULL)\n"
cor_in_line4 = "#define AI_NETWORK_OUT_SIZE (1 * 1 * 7)\n"

proj_dir = os.getcwd()
fh, abs_path = mkstemp()
nn_dir = os.path.join(proj_dir, 'NN_Code/Inc')
network_h_path = os.path.join(nn_dir, 'network.h')

# fix network.h file for compilation in Miosix OS
with os.fdopen(fh, mode='w') as new_f:

	with open(network_h_path) as old_f:
	
		for line in old_f:
			if re.search(pat1, line) != None:
				new_f.write(cor_in_line1)
			elif re.search(pat2, line) != None:
				new_f.write(cor_in_line2)
			elif re.search(pat3, line) != None:
				new_f.write(cor_in_line3)
			elif re.search(pat4, line) != None:
				new_f.write(cor_in_line4)
			elif re.search(pat5, line) != None:
				new_f.write("")
			elif re.search(pat6, line) != None:
				new_f.write("")
			else:
				new_f.write(line)

			
	old_f.close()
new_f.close()	

copymode(network_h_path, abs_path)
os.remove(network_h_path)
move(abs_path, network_h_path)

# remove old results of stm32ai
result_dir = os.path.join(proj_dir, 'stm32ai_results')
try:
    rmtree(result_dir)
except OSError as e:
    print("Error: %s - %s." % (e.filename, e.strerror))
	
	
# create results dir if not existing
if not os.path.exists(result_dir):
	os.makedirs(result_dir)



# move all stm32ai files into results directory
for file in os.listdir(proj_dir):
	if file.startswith('stm32ai_'):
		move(file, result_dir)


