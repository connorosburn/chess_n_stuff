function buildtests() {
  files=""
  for d in `find chess/lib/GameEngine -type d`
  do
    files="$files $(collectcpp $d)"
  done
  for d in `find chess/test -type d`
  do
    files="$files $(collectcpp $d)"
  done
  main="lib/main.cpp"
  files=${files//$main/}
  g++ -std=c++17 $files -o tests
}

function buildwasm() {
  files=""
  for d in `find chess/lib/GameEngine -type d`
  do
    files="$files $(collectcpp $d)"
  done
  for d in `find chess/lib/AI -type d`
  do
    files="$files $(collectcpp $d)"
  done
  for d in `find chess/lib/JavascriptBinding -type d`
  do
    files="$files $(collectcpp $d)"
  done
  main="lib/main.cpp"
  files=${files//$main/}
  echo $files
  em++ -std=c++17 -s ALLOW_MEMORY_GROWTH=1 --bind $files -o user_interface/wasmchess.js
}

function collectcpp() {
  files=""
  for c in $1/*.cpp; do
    files="$files $c"
  done
  echo $files
}

alias runtests='./tests'

function test() {
    buildtests
    runtests
    rm tests
}

function djangoinstall {
  cwd=$(pwd)
  cd multiplayer_backend
  source backenv/bin/activate
  pip3 install $1
  deactivate
  cd ..
}

function django() {
  cwd=$(pwd)
  cd multiplayer_backend
  source backenv/bin/activate
  python3 manage.py $1 --settings=multiplayer_backend.development
  deactivate
  cd ..
}

function backdev() {
  cwd=$(pwd)
  cd multiplayer_backend
  source backenv/bin/activate
  redis-server & python3 manage.py runserver --settings=multiplayer_backend.development
  deactivate
  cd ..
}

function frontdev() {
  cwd=$(pwd)
  cd user_interface
  npm start
  cd $cwd
}

function buildpythonbinding() {
  cwd=$(pwd)
  cmake -DPYTHON_EXECUTABLE:FILEPATH=/usr/bin/python3 .
  cmake --build .
  cd $cwd
}

function bundle() {
  cwd=$(pwd)
  cd multiplayer_backend
  source backenv/bin/activate
  pip3 freeze > requirements.txt
  deactivate
  zip ../chess_backend.zip -r * .[^.]*
  cd ..
}