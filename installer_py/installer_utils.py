
from tempfile import mkstemp
from shutil import move, copymode, rmtree
import os
import re

def fix_network_h(file_dir):
	pat1 = re.compile("#define AI_NETWORK_IN { \\\\")
	pat2 = re.compile("#define AI_NETWORK_IN_SIZE { \\\\")
	pat3 = re.compile("#define AI_NETWORK_OUT { \\\\")
	pat4 = re.compile("#define AI_NETWORK_OUT_SIZE { \\\\")
	pat5 = re.compile("\\\\")
	pat6 = re.compile("}")

	cor_in_line1 = "#define AI_NETWORK_IN "
	cor_in_line2 = "#define AI_NETWORK_IN_SIZE "
	cor_in_line3 = "#define AI_NETWORK_OUT "
	cor_in_line4 = "#define AI_NETWORK_OUT_SIZE "
	fh, abs_path = mkstemp()
	network_h_path = os.path.join(file_dir, 'network.h')

	# fix network.h file for compilation in Miosix OS
	with os.fdopen(fh, mode='w') as new_f:

		with open(network_h_path) as old_f:
			lines = old_f.readlines()
			for i in range(0,len(lines)):
				if re.search(pat1, lines[i]) != None:
					cor_line1 = cor_in_line1 + lines[i+1].replace('), \\', ')')
					new_f.write(cor_line1)
				elif re.search(pat2, lines[i]) != None:
					cor_line2 = cor_in_line2 + lines[i+1].replace('), \\', ')')
					new_f.write(cor_line2)
				elif re.search(pat3, lines[i]) != None:
					cor_line3 = cor_in_line3 + lines[i+1].replace('), \\', ')')
					new_f.write(cor_line3)
				elif re.search(pat4, lines[i]) != None:
					cor_line4 = cor_in_line4 + lines[i+1].replace('), \\', ')')
					new_f.write(cor_line4)
				elif re.search(pat5, lines[i]) != None:
					new_f.write("")
				elif re.search(pat6, lines[i]) != None:
					new_f.write("")
				else:
					new_f.write(lines[i])

				
		old_f.close()
	new_f.close()	

	copymode(network_h_path, abs_path)
	os.remove(network_h_path)
	move(abs_path, network_h_path)




	

