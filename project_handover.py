import os
import sys
import shutil


if __name__ == "__main__":
    proj_folder = './'
    target_folders = ['./birdApp']
    for target_folder in target_folders:
        shutil.rmtree(os.path.join(proj_folder, target_folder, 'bin'), ignore_errors=True)
        shutil.rmtree(os.path.join(proj_folder, target_folder, 'obj'), ignore_errors=True)

    os.chdir(proj_folder)
    output_folder = os.path.join(proj_folder, 'bin')

    if not os.path.exists(output_folder):
        os.mkdir(output_folder)
    
    os.system('tar czvf bin/project_phase3_group1_handover.tgz ' + ' '.join(target_folders))

    