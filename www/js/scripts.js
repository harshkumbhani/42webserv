var uploadModal = document.getElementById("fileUploadModal");
var deleteModal = document.getElementById("fileDeleteModal");
var deleteForm = document.getElementById("deleteForm");

// Function to open the upload modal
function openModal() {
	uploadModal.style.display = "block";
}

// Function to close the upload modal
function closeModal() {
	uploadModal.style.display = "none";
}

// Function to open the delete modal and load files dynamically
function openDeleteModal() {
	deleteModal.style.display = "block";
	loadFilesForDeletion();
}

// Function to close the delete modal
function closeDeleteModal() {
	deleteModal.style.display = "none";
}

// Function to handle clicking outside the modal to close it
window.onclick = function(event) {
	if (event.target == uploadModal) {
		closeModal();
	} else if (event.target == deleteModal) {
		closeDeleteModal();
	}
}

// Function to load files dynamically into the delete modal
function loadFilesForDeletion() {
	fetch('./www/upload/Files/')
		.then(response => {
			if (!response.ok) {
				throw new Error('Network response was not ok');
			}
			return response.json();
		})
		.then(files => {
			deleteForm.innerHTML = '';  // Clear previous list
			files.forEach(file => {
				deleteForm.innerHTML += `
					<input type="radio" name="fileToDelete" value="${file}"> ${file}<br>
				`;
			});
		})
		.catch(error => {
			console.error('Error loading files:', error);
			deleteForm.innerHTML = '<p>Failed to load files</p>';
		});
}


document.getElementById('deleteButton').addEventListener('click', function() {
	const selectedFile = document.querySelector('input[name="fileToDelete"]:checked');
	if (selectedFile) {
		const fileName = selectedFile.value;
		const url = `/delete?url=${encodeURIComponent(fileName)}`;

		fetch(url, {
			method: 'DELETE',
			headers: {
				'Content-Type': 'application/json'
			}
		})
		.then(response => {
			if (response.ok) {
				alert('File deleted successfully!');
				loadFilesForDeletion();  // Refresh the list of files
			} else if (response.status === 404) {
				alert('File not found.');
			} else {
				alert('Failed to delete the file.');
			}
		})
		.catch(error => {
			console.error('Error:', error);
			alert('An error occurred while deleting the file.');
		});
	} else {
		alert('Please select a file to delete.');
	}
});

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
