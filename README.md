# Simple Tetris

## 1. 개발 환경
* Editor : Visual Studio 2013
* Language : C++
* Engine : cocos2d-x 3.11.1
## 2. 조작법
* Left-Arrow : 왼쪽으로 이동
* Right-Arrow : 오른쪽으로 이동
* Up-Arrow : 오른쪽으로 회전
* Down-Arrow : 아래로 한칸 이동
* Space-Bar : 블럭을 맨 아래로 즉시 이동
## 3. 코드 설명
* Block.cpp : 제일 기본이 되는 한 블록 클래스
* Chunk.cpp : 플레이어가 조종할 블록들의 배열 클래스
* Field.cpp : 게임의 배경블록 배열 클래스. 블록이 더 이상 아래로 이동 할 수 없는 경우 그 자리의 Chunk가 Field로 귀속된다.
* Game.cpp : MVC구조에서 Control부분을 담당한다. 여러가지 체크나 위 모델들을 구현하고 처리를 담당한다.
* GameScene.cpp : View 와 Control부분을 담당한다. 화면이 어떻게 보여질지와 게임의 종료나 주기적인 업데이트 (게임루프)를 담당한다.
* Config.h : 게임내 상수를 설정한다.
## 4. 순서도
![순서도](/순서도.png)