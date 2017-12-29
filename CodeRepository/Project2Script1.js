// JavaScript source code

function HideUnhide(count) {
    var x = document.getElementById(count);
    if (x.style.display === 'none')
    {
        x.style.display = 'block';
    }
    else
    {
        x.style.display = 'none';
    }
}

function StartHidUnhide() {
    
    var x = document.getElementById('hideDIV');
    if (x.style.display === 'none')
    {
        x.style.display = 'block';
    }
    else {
        x.style.display = 'none';
    }
}
