/* Steer
 modified 11 JAN 2017
 by Yanan LI
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup()
{
    Serial.begin(9600); // BaudRate:9600
    myservo.attach(9);  // attaches the servo on pin 9
}

String comdata = "";
double Ang = 0; // the target angle steer will go to

void loop()
{
    comdata = "";

    while(Serial.available())
    {
        char ch = char(Serial.read());
        comdata += ch;
    }

    //Serial.println(comdata);

    // Trim the string to a style of "x...a"
    comdata = trimData(comdata);
    // cast "string" data to "double" data
    double angle = toInter(comdata);

    // Increment the value of Ang
    Ang += angle;
    // Modify the scope of Ang
    if (Ang > 180)
    {
        Ang = 180;
    }
    else if (Ang < 0)
    {
        Ang = 0;
    }
    //Serial.print("Ang: ");
    //Serial.println(Ang);

    // Run the steer engine to the target position
    myservo.write(Ang);
    delay(100);
}


// The original string may be like this: x23.4ax24.6ax28.3ax23.4ax23.4a
// Trim the original string to a kind of "x...a", such as "x23.4a"
String trimData(String temp)
{
    String comdata = "";
    int len = temp.length();

    // the process of trim
    for (int i = 0; i < len; i++)
    {
        if ('x' == temp[i])
        {
            while('a' != temp[i] && i < len)
            {
                comdata += temp[i++];
            }
            comdata += 'a';
            break;
        }
    }
    //Serial.print("new data:");
    //Serial.println(comdata);
    return comdata;
}

// Convert a string to a float number
double toInter(String comdata)
{
    int index = 0;
    bool isNegative = false; // true：the value is negative；false：positive
    bool hasDot = false; // true：the value has a point  false：have no point
    int dotPos = 0; // the index of point
    int signPos = 0; // the index of negative sign
    while(index < comdata.length())
    {
        int num = int(comdata[index])-48;

        // negative not not
        if (-3 == num) // comdata is a negatve value
        {
            isNegative = true;
            signPos = index;
            //Serial.print("negetive position:");
            //Serial.println(index);
        }

        // a decimal point
        if (-2 == num) // comdata has a decimal point
        {
            hasDot = true;
            dotPos = index;
            //Serial.print("dot position:");
            //Serial.println(dotPos);
        }
        //Serial.println(num);
        index += 1;
    }

    double angle = 0;
    int t = 0;
    double dec = 1.0; //0.1/0.01等待

    // If the value has a point, calcilate the right side value of point
    if (hasDot)
    {
        for (int i = 1; i <= comdata.length()-dotPos-2; i++)
        {
            dec *= 0.1;
            angle += dec * int(comdata[dotPos + i]-48);
        }
    }
    else // 没有小数点
    {
        dotPos = comdata.length()-1;
    }

    int No = 0; // 个、十、百的位数
    // Negative or not
    if (isNegative)
    {
        No = dotPos - 2;
    }
    else
    {
        No = dotPos - 1;
    }

    int dec2 = 1;
    for (int j = 1; j <= No; j++)
    {
        angle += dec2 * int(comdata[dotPos - j]-48);
        dec2 *= 10;
    }

    // if angle is a negetive number
    if (isNegative)
    {
        angle *= -1;
    }

    // the final result
    //Serial.print("result angle:");
    //Serial.println(angle);
    return angle;
}

