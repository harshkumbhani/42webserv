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

function loadDirectoryListing(path) {
	document.getElementById('buttons').style.display = 'none';
	fetch(path)
	.then(response => {
		const contentType = response.headers.get('Content-Type');
		const isFile = response.headers.get('X-File-Type') === 'file';

		if (contentType && contentType.startsWith('text/html') && !isFile) {
			return response.text().then(htmlSnippet => {
				document.getElementById('tableBody').innerHTML = htmlSnippet;
				document.getElementById('directory-listing').style.display = 'block';
				addDirectoryLinkListeners();  // Re-add listeners to the newly loaded content
			});
		} else {
			const fileURL = response.url;
			window.open(fileURL, '_blank');  // Open the file in a new tab
		}
	})
	.catch(error => {
			console.error('Error loading directory listing:', error);
	});
}

function closeDirectoryListing() {
	document.getElementById('directory-listing').style.display = 'none';
	document.getElementById('buttons').style.display = 'block';
	document.getElementById('tableBody').innerHTML = '';
}

function addDirectoryLinkListeners() {
	document.querySelectorAll('#tableBody a').forEach(link => {
		link.addEventListener('click', function (event) {
			event.preventDefault();  // Prevent default link behavior
			loadDirectoryListing(this.getAttribute('href'));  // Load the clicked directory
			});
			});
			}
			
			// Initial setup
			document.addEventListener('DOMContentLoaded', function() {
				addDirectoryLinkListeners();
				});
				
function loadGetPage() {
	// Hide the buttons
	document.getElementById('buttons').style.display = 'none';
	console.log(" I am here in GET");
	
	// Load the content of getPage.html
	fetch('/pages/getPage.html')
	.then(response => response.text())
	.then(html => {
		document.getElementById('content-placeholder').innerHTML = html;

		// Re-run any scripts within the loaded HTML
		const scripts = document.querySelectorAll('#content-placeholder script');
		scripts.forEach(oldScript => {
			const newScript = document.createElement('script');
			newScript.textContent = oldScript.textContent;
			oldScript.replaceWith(newScript);
		});
	})
	.catch(error => {
			console.error('Error loading GET page:', error);
	});
}

function loadGetCGI() {
	// Hide the buttons
	document.getElementById('buttons').style.display = 'none';

	// Load the content of cgi.html
	fetch('/pages/cgi.html')
	.then(response => response.text())
	.then(html => {
			document.getElementById('content-placeholder').innerHTML = html;

			// Re-run any scripts within the loaded HTML
			const scripts = document.querySelectorAll('#content-placeholder script');
			scripts.forEach(oldScript => {
					const newScript = document.createElement('script');
					newScript.textContent = oldScript.textContent;
					oldScript.replaceWith(newScript);
			});
	})
	.catch(error => {
			console.error('Error loading CGI page:', error);
	});
}

const goBack = () => {
	document.getElementById('directory-listing').style.display = 'none';
	document.getElementById('content-placeholder').innerHTML = '';
	document.getElementById('buttons').style.display = 'block';
};

const getImage = async () => {
	try {
			const response = await fetch('/get-files');

			if (!response.ok) {
					throw new Error(`HTTP error! Status: ${response.status}`);
			}

			const blob = await response.blob();
			const imageUrl = URL.createObjectURL(blob);
			const img = document.getElementById('getImage');

			img.src = imageUrl;
			img.style.display = 'block';
			img.onload = () => URL.revokeObjectURL(imageUrl);  // Clean up the object URL after loading
	} catch (error) {
			console.error('Error:', error);
			alert('Error: ' + error.message);
	}
};

function showTeapotModal() {
	window.location.href='./../.troll/troll.html';
}

function closeTeapotModal() {
	window.history.back();
}


const display = (whatToDisplay) => {
	if (Math.random() < 1.0) { // 20% chance
		showTeapotModal();
	} else if (whatToDisplay === 'get') {
		loadGetPage();
	} else if (whatToDisplay === 'post') {
		openModal();
	} else if (whatToDisplay === 'upload') {
		loadDirectoryListing('/upload');
	} else if (whatToDisplay === 'cgi') {
		loadGetCGI();
	} else if (whatToDisplay === 'redirect') {
		window.location.href='/redirect';
	}
}