# ************************   NEURAL NETWORK INSTALLER   **********************************

import os
from distutils import dir_util, file_util
import shutil
import tempfile
from installer_utils import *
from installer_settings import *
import sys
		  
			  
			  
proj_dir = sys.argv[1]
miosix_dir = os.path.join(proj_dir, "miosix-kernel")
ai_module_dir = os.path.join(miosix_dir, 'AI')

if not os.path.exists(ai_module_dir):
	os.makedirs(ai_module_dir)

nncode_dir = os.path.join(proj_dir, 'NN_Code')
installer_dir = os.path.join(proj_dir, 'installer_py')
adds_dir = os.path.join(installer_dir, 'additional_files')


# Install NN files in Miosix kernel
for itm in os.listdir(nncode_dir):
	if itm in nn_folders:
		src_path = os.path.join(nncode_dir, itm)
		dst_path = os.path.join(ai_module_dir, itm)
		
		if os.path.isdir(src_path):
			dir_util.copy_tree(src_path, dst_path)
		else:
			file_util.copy_file(src_path, dst_path)
	
	
# Install all additional files in Miosix kernel	
for itm in os.listdir(adds_dir):
	if itm in adds:
		src_path = os.path.join(adds_dir, itm)
		dst_path = os.path.join(ai_module_dir, itm)
		
		if os.path.isdir(src_path):
			dir_util.copy_tree(src_path, dst_path)
		else:
			file_util.copy_file(src_path, dst_path)
	
aimod_inc_dir = os.path.join(ai_module_dir, 'Inc')
fix_network_h(aimod_inc_dir)


white_list = set(h_files + c_files + cpp_files)


# Clean installation directory
ai_module_inc_dir = os.path.join(ai_module_dir, 'Inc')

for itm in os.listdir(ai_module_inc_dir):
	if itm not in white_list:
		itm_path = os.path.join(ai_module_dir, 'Inc/'+itm)
		os.remove(itm_path)

ai_module_src_dir = os.path.join(ai_module_dir, 'Src')

for itm in os.listdir(ai_module_src_dir):
	if itm not in white_list:
		itm_path = os.path.join(ai_module_dir, 'Src/'+itm)
		os.remove(itm_path)




