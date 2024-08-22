

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

//function loadDeleteListing() {
//	window.location.href = './../delete_listing.html';
//}
