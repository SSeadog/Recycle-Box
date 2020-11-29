# Recycle-Box

![테스트](https://user-images.githubusercontent.com/58541838/100490861-640c0f00-3162-11eb-957c-3499ebdd38a3.gif)

## 프로젝트 구상

> 요즘 일회용품 사용이 증가하고 있는 반면, 분리수거는 제대로 이뤄지지 않고 있습니다. 이를 해결하는데 힘을 싣고자 자동 분리수거 박스를 구상하게 되었습니다.

## 프로젝트 계획

> 딥러닝(yolov5 모델)을 통해 객체를 판별하고 이를 아두이노를 이용해 이동시키도록 계획했습니다.

## 프로젝트 구현

![구현ppt](https://user-images.githubusercontent.com/58541838/99402841-2373f080-292d-11eb-8cdf-6f42bfa134ff.png)

> 1. 아두이노에서 적외선 센서를 이용해 물체가 들어옴을 인식하고 파이썬으로 통신
> 2. 파이썬은 통신을 받으면 해당 물체를 yolov5모델을 통해 어떤 재활용품인지 판별하고 아두이노에게 전달
> 3. 아두이노는 전달받은대로 모터를 움직여 물체를 해당 쓰레기통으로 옮김

## 구현 상세

### 아두이노
1. 다중모터 제어를 위한 라이브러리 추가
``` c
#include <Adafruit_PWMServoDriver.h>

```
2. 가독성을 위해 모터 제어 함수 모듈화
``` c
void move1() { // 4개의 칸 중 첫 번째 칸으로 옮기는 함수
  for(uint16_t pulselen = 275; pulselen < 480; pulselen+=2) { // 0번 모터 오른쪽으로 돌리기
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }
  delay(200);
  ... // 0번 모터 원위치
  ... // 2번 모터 오른쪽으로 돌리기
  ... // 2번 모터 원위치
}
```
3. 모터 제어 함수들을 편하게 사용하기 위해 moveTo함수 추가
``` c
void moveTo(char n) {
  if(n == '1') {
    moveTo1();
  } else if(n == '2') {
    moveTo2();
  } else if(n == '3') {
    moveTo3();
  } else {
    moveTo4();
  }
}
```
4. 프로그램의 각 단계들을 state로 관리
``` c
void setup() { // setup에서 각 state 초기화
  st1 = true;
  st2 = false;
  st3 = false;
}

void loop() {
  if(st1) {
  ... // 초음파 센서로부터 물체 투입을 감지해서 distance에 거리 값을 cm단위로 저장
    if(distance <= 20) { // 거리 값이 20cm이하이면(물체가 투입되면)
      Serial.println("1"); // python으로 '1' 전송
      st2 = true; // 2단계를 수행하기 위해 st2를 true로 변경
      st1 = false; // 1단계를 종료하기 위해 st1을 false로 변경
    }
  }
  
  if(st2) {
    if(Serial.available()) { // 값이 입력되면(python에서 값을 보내주면)
      kind = Serial.read(); // 받은 값을 kind에 저장
      Serial.read(); // 줄넘김 문자는 버림

      st3 = true; // 3단계 수행을 위해 st3를 true로 변경
      st2 = false; // 2단계 종료 위해 st2를 false로 변경
    }
  }
  
  if(st3) {
    moveTo(kind); // 모터를 움직이는 moveTo함수를 kind를 넘겨주며 호출
    st3 = false; // 3단계 종료를 위해 st3를 false로 변경
    st1 = true; // 1단계 수행을 위해 st1을 true로 변경
  }
}
```

### 파이썬
* yolov5의 detect.py를 분석하여 필요한 부분을 추가했습니다.
* 각종 초기 작업 후에 webcam으로 판별하는 경우 for문을 통해 영상의 각 프레임마다 Inference가 이루어지는 것을 알아냈습니다.
``` python

# ...초기작업...

for path, img, im0s, vid_cap in dataset:
    # 이 반복문 안에서 Inference, detections, Write results ... 등의 작업이 이루어짐
  
```
1. 아두이노에서 값을 전달받았을 때만 판별작업이 이루어질 수 있도록 if문을 추가했습니다.
``` python
for path, img, im0s, vid_cap in dataset:
    if ARD.readable(): # readable을 통해 값을 받을 수 있으면
        cur = ARD.readline().decode().strip()
        if cur == "1" or cnt != 0:
            # Inference, detections, Write results ... 등의 작업 수행
            .
            .
            .
```
2. 판별은 한번에 10번까지만 이루어지도록 하였으며 10번의 결과를 조합해 아두이노로 전송하도록 하였습니다.
``` python
cnt = 0 # 현재 반복한 횟수를 저장할 변수. 0으로 초기화
mat = [0, 0, 0, 0]  # can pls gls trsh가 몇번씩 나왔는지 저장할 변수. 각각 0으로 초기화
for path, img, im0s, vid_cap in dataset:
    if ARD.readable():
        cur = ARD.readline().decode().strip()
        if cur == "1" or cnt != 0: // cnt가 0이 아닐때 실행
            cnt += 1 // cnt 1증가
            
            # Inference, NMS, Classifier 작업이 이루어짐
            
            # Process detections
                for i, det in enumerate(pred):  # detections per image                 
                    .
                    .
                    .
                    
                    if det is not None and len(det):
                        .
                        .
                        .
                        
                        for c in det[:, -1].unique(): # detections per class
                            n = (det[:, -1] == c).sum()
                            # add to string
                            s += '%g %ss, ' % (n, names[int(c)])
                            # 여기서 c에 판별된 클래스가 들어있음. 판별된 객체를 mat[int(c)]에 추가해줌
                            mat[int(c)] += 1
                    else:  # det이 None일 때 can pls gls로 인식되지 않았음을 의미하므로 trsh 1 증가
                        mat[3] += 1    
            .
            .
            .
            
            if cnt == 10: # 횟수가 10이 되면
                cnt = 0 # 0으로 초기화
                # can, pls, gls, trsh 중 젤 많이 나온 것을 max_mat에 저장
                max_mat = -1
                for i, m in enumerate(mat):
                    if m > max_mat:
                        max_mat = i
                # max_mat를 아두이노로 전송. 아두이노에서 0을 받으면 아무것도 받지 않았다고 인식되어 1을 더하여 전송
                ARD.write(max_mat+1)
```
3. 아두이노에서 값을 전송하지 않았을 때도 비디오를 확인하기 위해 else문 추가
``` python
for path, img, im0s, vid_cap in dataset:
    if ARD.readable():
        .
        .
        .
        
    else:
        if view_img:
            cv2.imshow("video", im0s[0].copy())
            if cv2.waitKey(1) == ord('q'):  # q to quit
                raise StopIteration
```

## 시행착오

> ### 1. 아두이노와 파이썬 통신
>
> * 아두이노 보드에서 실행되는 프로그램과 노트북에서 실행되는 프로그램이 서로 정보를 주고 받으려면 어떻게 해야할까 고민을 했는데, Serial 통신으로 해결할 수 있었습니다.
>
> ### 2. 아두이노의 쓰레드 미지원
>
> * 아두이노에서 여러 작업을 동시에 실행시키고 싶었는데 아두이노는 멀티 태스킹을 위한 쓰레드를 지원하지 않았습니다. 이를 해결하기 위해서 각 작업들을 state에 따라 실행될 수 있도록 구현하여 해결할 수 있었습니다.
