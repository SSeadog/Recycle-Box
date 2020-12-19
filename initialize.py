# yolov5를 설치하신 후 이 파일을 실행해 주세요
import subprocess

subprocess.run(['move', 's_detect.py', 'yolov5'], shell=True)
# source 값에 사용하실 자료를 넣어주세요(0 웹캠, ./경로/이미지이름.확장자 ...)
# weight에는 사용하실 weight를 넣어주세요
# img는 출력 이미지, 영상의 크기이며 conf는 그 값부터 화면에 표시하겠다는 말입니다(0.5면 50%의 정확성부터 화면에 표시)
subprocess.run(
    'python ./yolov5/s_detect.py --source 0 --weight weights/can_pet_plscup.pt --img 416 --conf 0.5', shell=True)
