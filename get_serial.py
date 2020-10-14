import serial
import cv2

print('serial'+serial.__version__)

# set a port number & baud rate
PORT = 'COM3'  # 아두이노 열결후 몇번 포트로 연결되는지 확인하고 적어야댐
BaudRate = 9600

ARD = serial.Serial(PORT, BaudRate)  # 시리얼 톹신을 위한 설정, 선언


def Decode(A):  # 시리얼 한줄 읽은 후 결과 리턴
    A = A.decode().strip()
    if A == '1':
        return True
    else:
        return False


def Ardread():  # ARD 읽어서 Decode 실행 후 결과 리턴
    cur = ARD.readline()
    if cur:
        LINE = cur
        if Decode(LINE) and is_ready:
            print("Decode True")
            # 화면 캡처 후 yolov5 detect.py 실행해서 결과 만들기
            # detect.py의 결과를 인식하면 0~3사이의 값을 반환하도록 수정해야함
            # 결과를 아두이노로 전송 0 캔 1 플라스틱 2 유리 3 일반쓰레기
            # frame엔 비디오의 한프레임이 들어가 있는데 타입이 numpy.ndarray라고 나옴. source값으로 줄 수 있는지 모르겠음
            # frame을 이미지파일로 저장한 후 넘겨줘야할듯
        # return 0
        else:
            print("NOT 1")


# 0은 기본 웹캠을 의미, 두번째 인자는 async관련 warn제거를 위해 넣음
capture = cv2.VideoCapture(0, cv2.CAP_DSHOW)
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 416)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 416)

while True:
    # frame만으로 detect.py의 소스로 넘겨줄 수 있나?
    ret, frame = capture.read()
    print(type(frame))
    cv2.imshow("VideoFrame", frame)
    Ardread()
    if cv2.waitKey(1) == ord('q'):  # q 입력시 종료
        break

# 카메라 할당 메모리 해제, 윈도우창 닫음
capture.release()
cv2.destroyAllWindows()
