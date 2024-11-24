
# RVC Controller Implementation

이 프로젝트에서는 RVC Controller 로직과 Unity 테스트 프레임워크를 활용한 RVC Controller의 테스트 케이스를 구현하였습니다.

## 주요 파일

- **sw_team_test.c** : RVC Controller 테스트 케이스 구현 파일
- **sw_team.c** : RVC Controller 로직 구현 파일
- **unity.c** : Unity 테스트 프레임워크 소스 파일
- **unity.h** : Unity 테스트 프레임워크 헤더 파일
- **sw_team.h** : RVC Controller 로직의 헤더 파일

## 테스트 파일(sw_team_test.c) 빌드 및 실행 방법

1. GCC를 사용하여 프로젝트를 컴파일합니다.

   ```bash
   gcc -o sw_team_test sw_team_test.c sw_team.c unity.c -I. -g
   ```

2. 테스트 실행 파일을 실행합니다.

   ```bash
   ./sw_team_test
   ```
