// const textEndpointUrl = '/api/text.txt';
// const fileBaseEndpointUrl = '/api/';
//
// function submitText() {
//     const input = document.getElementById('input').value;
//     sendData(input, textEndpointUrl);
// }
//
// function submitImage() {
//     const fileInput = document.getElementById('image');
//     const file = fileInput.files[0];
//     sendData(file, fileBaseEndpointUrl + encodeURIComponent(file.name));
// }
//
// function sendData(data, endpointUrl) {
//     const xhr = new XMLHttpRequest();
//     xhr.open('POST', endpointUrl, true);
//     xhr.onload = function () {
//         if (this.status === 200) {
//             document.getElementById('output').innerHTML = this.responseText;
//         }
//     };
//
//     if (data instanceof File) {
//         xhr.setRequestHeader('Content-Type', data.type);
//         xhr.setRequestHeader('X-File-Name', data.name);
//         xhr.send(data);
//     } else {
//         xhr.setRequestHeader('Content-Type', 'text/plain');
//         xhr.setRequestHeader('X-File-Name', 'text.txt');
//         xhr.send(data);
//     }
// }
//
// function deleteText() {
//     const textDiv = document.getElementById('textToDelete');
//     const xhr = new XMLHttpRequest();
//     xhr.open('DELETE', '/api/upload/text.txt', true);
//     xhr.send();
// }

const textEndpointUrl = '/api/text.txt';
const fileBaseEndpointUrl = '/api/';

function submitText() {
    const input = document.getElementById('input').value;
    sendData(input, textEndpointUrl);
}

function submitImage() {
    const fileInput = document.getElementById('image');
    const file = fileInput.files[0];
    sendData(file, fileBaseEndpointUrl + encodeURIComponent(file.name));
}

function sendData(data, endpointUrl) {
    const xhr = new XMLHttpRequest();
    xhr.open('POST', endpointUrl, true);
    xhr.onload = function() {
        if (this.status === 200) {
            document.getElementById('output').innerHTML = this.responseText;
        }
    };

    if (data instanceof File) {
        xhr.setRequestHeader('Content-Type', data.type);
        xhr.setRequestHeader('X-File-Name', data.name);
        xhr.send(data);
    } else {
        xhr.setRequestHeader('Content-Type', 'text/plain');
        xhr.setRequestHeader('X-File-Name', 'text.txt');
        xhr.send(data);
    }
}

function deleteText() {
    const textDiv = document.getElementById('textToDelete');
    const xhr = new XMLHttpRequest();
    xhr.open('DELETE', '/api/upload/text.txt', true);
    /* xhr.onload = function() {
        if (this.status === 200) {
            textDiv.innerHTML = ''; // Remove the text from the DOM
        }
    }; */
    xhr.send();
}