$(document).ready(function() {
    var inputs = [];
    var temp = '';
    var total = 0;
    $('button').on('click', function () {
        var value = $(this).text();
        if (!isNaN(value) || value === '.') 
        {
            temp += value;
            $('#display').val(temp.substring(0, 10));
        } 
        else if (value === 'AC') 
        {
            inputs = [];
            temp = '';
            total = 0;
            $('#display').val('');
        } 
        else if (value === 'CE') 
        {
            temp = '';
            $('#display').val('');
        } 
        else if (value === 'x') 
        {
            inputs.push(temp);
            inputs.push('*');
            temp = '';
        }
        else if (value === '\xF7') 
        {
            inputs.push(temp);
            inputs.push('/');
            temp = '';
        } 
        else if (value === '=') 
        {
            inputs.push(temp);
            var number = Number(inputs[0]);
            for (var i = 1; i < inputs.length; i=i+2) 
            {    
                var nextNum = Number(inputs[i + 1]);
                var symbol = inputs[i];
                if (symbol === '+') 
                {
                    number += nextNum;
                } 
                else if (symbol === '-') 
                {
                    number -= nextNum;
                } 
                else if (symbol === '*') 
                {
                    number *= nextNum;
                } 
                else if (symbol === '/') 
                {
                    number /= nextNum;
                } 
                else if (symbol === '%') 
                {
                    nextNum=Math.abs(nextNum);
                    number %= nextNum;
                    number = Math.abs(number);
                }
            }
            if (number < 0) 
            {
                number = Math.abs(number) + '-';
            }
            $('#display').val(number);
            inputs = [];
            temp = '';
        } 
        else 
        {
            inputs.push(temp);
            inputs.push(value);
            temp = '';
        }
    });
});