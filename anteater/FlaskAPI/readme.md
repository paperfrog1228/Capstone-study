# Quickstart for Flask API Server

### 가상환경 진입 명령어
git bash :
```
source .env/Scripts/activate
```

cmd :
```
.env\Scripts\activate
```

### 가상환경 종료 명령어
cmd :
```
.env\Scripts\deactivate.bat
```

## APIs
 * `GET /user`
 * `POST /user`
 * `GET /user/<userName>`
 * `POST /auth/login`
 * `POST /auth/logout`

설명은 생략한다.

## References
 1. https://blog.thereis.xyz/94
 2. https://dejavuqa.tistory.com/273 : Flask로 API 서버 만들기 ([원문](https://www.freecodecamp.org/news/structuring-a-flask-restplus-web-service-for-production-builds-c2ec676de563/)), 몇 년 지난 자료라는 점 감안해야함.
 3. https://pymongo.readthedocs.io/en/stable/tutorial.html : PyMongo documentation
 4. https://pymodm.readthedocs.io/en/stable/ : ORM for PyMongo