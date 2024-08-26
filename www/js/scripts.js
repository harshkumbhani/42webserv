

var modal = document.getElementById("fileUploadModal");

function openModal() {
	modal.style.display = "block";
}

function closeModal() {
	modal.style.display = "none";
}

window.onclick = function(event) {
	if (event.target == modal) {
		modal.style.display = "none";
	}
}

document.addEventListener('click', function() {
    var audio = document.getElementById('notification');
    audio.muted = false;
    audio.play().catch(function(error) {
        console.error('Audio playback failed:', error);
    });
}, { once: true });

// CGI 
var cgimodal = document.getElementById("fileUploadModal");

function openModalCgi() {
	cgimodal.style.display = "block";
}

function closeModalCgi() {
	cgimodal.style.display = "none";
}

window.onclick = function(event) {
	if (event.target == cgimodal) {
		cgimodal.style.display = "none";
	}
}
