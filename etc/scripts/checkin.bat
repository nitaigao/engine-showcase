
echo Enter Commit Message: 
set /p message=

PATH %PATH%;C:\Program Files\Git\bin\

set error=

rake -f clean.rb
rake -f clean.rb
cd ../../
git add .
git commit -a -m "%message%"
git push origin master
