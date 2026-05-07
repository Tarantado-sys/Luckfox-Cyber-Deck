#!/bin/sh

##################################################################################
# This Script auto configures your luckfox for UART3 communiaction upon startup. #
##################################################################################

echo "setuart3.sh is being created..."
touch /usr/bin/setuart3.sh

echo "setuart3.sh is created."

echo "writing to setuart3.sh..."
cat << EOF > /usr/bin/setuart3.sh
#!/bin/sh

stty -F /dev/ttyS3 921600

(echo TEXT_BEGIN; echo "Initializing UART3_M1"; echo TEXT_END) > /dev/ttyS3
sleep 1

(echo TEXT_BEGIN; echo "Init done!"; echo TEXT_END) > /dev/ttyS3
sleep 2

echo "CLEAR" > /dev/ttyS3
EOF

echo "giving permission >> /usr/bin/setuart3.sh"
chmod +x /usr/bin/setuart3.sh

echo "permission given..."

echo "writing to 'etc/init.d/rcS'..."
echo '/usr/bin/setuart3.sh' >> /etc/init.d/rcS

echo "DONE! ENJOY!"
echo "            a8888b."
echo "           d888888b."
echo "           8P\"YP\"Y88"
echo "           8|o||o|88"
echo "           8'    .88"
echo "           8\`._.' Y8."
echo "          d/      \`8b."
echo "         dP   .    Y8b."
echo "        d8:'  \"  \`::88b"
echo "       d8\"         'Y88b"
echo "      :8P    '      :888"
echo "       8a.   :     _a88P"
echo "     ._/\"Yaa_:   .| 88P|"
echo "jgs  \\    YP\"    \`| 8P  \`."
echo "a:f  /     \\.___.d|    .'"
echo "     \`--..__)8888P\`._.' "