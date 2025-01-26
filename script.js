document.addEventListener('DOMContentLoaded', () => {
    fetch('unsolved_problems.json')
        .then(response => response.json())
        .then(data => {
            window.problemsData = data;
            populateUserSelector(data);
            displayProblemsForUser();
        })
        .catch(error => console.error('Error fetching problems:', error));
});

function populateUserSelector(data) {
    const userSelector = document.getElementById('username');
    userSelector.innerHTML = '';

    const defaultOption = document.createElement('option');
    defaultOption.value = '';
    defaultOption.textContent = 'Select a friend';
    userSelector.appendChild(defaultOption);

    for (const friend of Object.keys(data)) {
        const option = document.createElement('option');
        option.value = friend;
        option.textContent = friend;
        userSelector.appendChild(option);
    }
}

function displayProblemsForUser() {
    const selectedUser = document.getElementById('username').value;
    const sortBy = document.getElementById('sort').value;
    let filteredData = {};

    if (selectedUser) {
        filteredData[selectedUser] = window.problemsData[selectedUser];
    } else {
        filteredData = window.problemsData;
    }

    const sortedData = sortProblems(filteredData, sortBy);
    displayProblems(sortedData);
    updateHistogram(sortedData);
}

function displayProblems(data) {
    const container = document.getElementById('problems-container');
    container.innerHTML = '';

    for (const [friend, problems] of Object.entries(data)) {
        const friendSection = document.createElement('div');
        friendSection.classList.add('friend-section');

        const friendTitle = document.createElement('h2');
        friendTitle.textContent = `Problems solved by ${friend}`;
        friendSection.appendChild(friendTitle);

        problems.forEach(problem => {
            const problemElement = document.createElement('div');
            problemElement.classList.add('problem');
            problemElement.innerHTML = `<a href="${problem.url}" target="_blank">${problem.name}</a> (Rating: ${problem.rating || 'N/A'})`;

            const tagsContainer = document.createElement('div');
            tagsContainer.classList.add('tags-container');
            problem.tags.forEach(tag => {
                const tagElement = document.createElement('span');
                tagElement.classList.add('tag');
                tagElement.textContent = tag;
                tagsContainer.appendChild(tagElement);
            });

            problemElement.appendChild(tagsContainer);
            friendSection.appendChild(problemElement);
        });

        container.appendChild(friendSection);
    }
}

function sortProblems(data, sortBy) {
    let sortedData = {};

    if (sortBy === 'friend') {
        sortedData = data;
    } else if (sortBy === 'rating-asc' || sortBy === 'rating-desc') {
        for (const [friend, problems] of Object.entries(data)) {
            sortedData[friend] = problems.slice().sort((a, b) => {
                if (a.rating === null) return 1;
                if (b.rating === null) return -1;
                return sortBy === 'rating-asc' ? a.rating - b.rating : b.rating - a.rating;
            });
        }
    }

    return sortedData;
}

function updateHistogram(data) {
    const ratings = [];

    for (const problems of Object.values(data)) {
        problems.forEach(problem => {
            if (problem.rating !== null) {
                ratings.push(problem.rating);
            }
        });
    }

    const ratingCounts = ratings.reduce((acc, rating) => {
        acc[rating] = (acc[rating] || 0) + 1;
        return acc;
    }, {});

    const labels = Object.keys(ratingCounts).sort((a, b) => a - b);
    const counts = labels.map(label => ratingCounts[label]);

    const ctx = document.getElementById('difficultyChart').getContext('2d');
    if (window.difficultyChart) {
        window.difficultyChart.destroy();
    }
    window.difficultyChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: [{
                label: 'Number of Problems',
                data: counts,
                backgroundColor: 'rgba(75, 192, 192, 0.2)',
                borderColor: 'rgba(75, 192, 192, 1)',
                borderWidth: 1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Number of Problems'
                    }
                },
                x: {
                    title: {
                        display: true,
                        text: 'Problem Rating'
                    }
                }
            }
        }
    });
}