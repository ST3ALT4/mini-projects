#include <iostream>
#include <string>

int calculate(const std::string&);

int main()
{
    std::string problem;
    std::cin >> problem;

    int sol = calculate(problem);

    std::cout << sol << std::endl;

    return 0;
}

int calculate(const std::string& nums)
{
    int ans = 0;
    char oper = '+';

    for (int j = 0; j < nums.size(); ++j)
    {
        if (nums[j]>='0' && nums[j]<='9')
        {
            int num = nums[j] - '0';
            
            switch (oper)
            {
            case '+':
                ans += num;
                break;
            case '-':
                ans -= num;
                break;
            case '*':
                ans *= num;
                break;
            case '/':
                if (num != 0)
                    ans /= num;
                else
                    std::cerr << "Division by zero error" << std::endl;
                break;
            default:
                break;
            }
        }
        else if (nums[j] == '+' || nums[j] == '-' || nums[j] == '*' || nums[j] == '/')
        {
            oper = nums[j];
        }
        else
        {
            std::cerr << "Invalid character encountered: " << nums[j] << std::endl;
            return 0;
        }
    }

    return ans;
}
