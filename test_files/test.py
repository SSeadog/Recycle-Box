import subprocess

subprocess.run(
    'python ./yolov5/detect.py --source 0 --weight weights/can_pet_plscup.pt --img 416 --conf 0.5', shell=True)
