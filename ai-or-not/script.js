// Original Images Array
const images = [
    { src: 'images/game/batch1/1-n.png', isAI: false },
    { src: 'images/game/batch1/2-n.png', isAI: false },
    { src: 'images/game/batch1/3-n.png', isAI: false },
    { src: 'images/game/batch1/4-n.png', isAI: false },
    { src: 'images/game/batch1/5-n.png', isAI: false },
    { src: 'images/game/batch1/6-n.png', isAI: false },
    { src: 'images/game/batch1/7-n.png', isAI: false },
    { src: 'images/game/batch1/8-y.png', isAI: true },
    { src: 'images/game/batch1/9-n.png', isAI: false },
    { src: 'images/game/batch1/10-y.png', isAI: true },
    { src: 'images/game/batch1/11-n.png', isAI: false },
    { src: 'images/game/batch1/12-y.png', isAI: true },
    { src: 'images/game/batch1/13-n.png', isAI: false },
    { src: 'images/game/batch1/14-n.png', isAI: false },
    { src: 'images/game/batch1/15-y.png', isAI: true },
    { src: 'images/game/batch1/16-n.png', isAI: false },
    { src: 'images/game/batch1/17-n.jpg', isAI: false },
    { src: 'images/game/batch1/18-y.png', isAI: true },
    { src: 'images/game/batch1/19-y.png', isAI: true },
    { src: 'images/game/batch1/20-y.png', isAI: true },
    //batch 2
    { src: 'images/game/batch2/1-n.png', isAI: false },
    { src: 'images/game/batch2/2-y.png', isAI: true },
    { src: 'images/game/batch2/3-y.png', isAI: true },
    { src: 'images/game/batch2/4-n.png', isAI: false },
    { src: 'images/game/batch2/5-y.png', isAI: true },
    { src: 'images/game/batch2/6-n.png', isAI: false },
    { src: 'images/game/batch2/7-y.png', isAI: true },
    { src: 'images/game/batch2/8-n.png', isAI: false },
    { src: 'images/game/batch2/9-y.png', isAI: true },
    { src: 'images/game/batch2/10-y.png', isAI: true },
    { src: 'images/game/batch2/11-y.png', isAI: true },
    { src: 'images/game/batch2/12-y.png', isAI: true },
    { src: 'images/game/batch2/13-y.png', isAI: false },
    { src: 'images/game/batch2/14-y.png', isAI: true },
    { src: 'images/game/batch2/15-y.png', isAI: true },
    { src: 'images/game/batch2/16-n.png', isAI: false },
    { src: 'images/game/batch2/17-y.png', isAI: true },
    { src: 'images/game/batch2/18-y.png', isAI: true },
    { src: 'images/game/batch2/19-y.png', isAI: true },
    { src: 'images/game/batch2/20-n.png', isAI: false },
    //batch 3
    { src: 'images/game/batch3/1-y.png', isAI: true },
    { src: 'images/game/batch3/2-n.png', isAI: false },
    { src: 'images/game/batch3/3-y.png', isAI: true },
    { src: 'images/game/batch3/4-n.png', isAI: false },
    { src: 'images/game/batch3/5-n.png', isAI: false },
    { src: 'images/game/batch3/6-n.png', isAI: false },
    { src: 'images/game/batch3/7-n.png', isAI: false },
    { src: 'images/game/batch3/8-y.png', isAI: true },
    { src: 'images/game/batch3/9-n.png', isAI: false },
    { src: 'images/game/batch3/10-y.png', isAI: true },
    { src: 'images/game/batch3/11-n.png', isAI: false },
    { src: 'images/game/batch3/12-n.png', isAI: false },
    { src: 'images/game/batch3/13-y.png', isAI: true },
    { src: 'images/game/batch3/14-y.png', isAI: true },
    { src: 'images/game/batch3/15-n.png', isAI: false },
    { src: 'images/game/batch3/16-y.png', isAI: true },
    { src: 'images/game/batch3/17-y.png', isAI: true },
    { src: 'images/game/batch3/18-n.png', isAI: false },
    { src: 'images/game/batch3/19-n.png', isAI: false },
    { src: 'images/game/batch3/20-y.png', isAI: true },
    //batch 4
    { src: 'images/game/batch4/1-y.png', isAI: true },
    { src: 'images/game/batch4/2-y.png', isAI: true },
    { src: 'images/game/batch4/3-y.png', isAI: true },
    { src: 'images/game/batch4/4-n.png', isAI: false },
    { src: 'images/game/batch4/5-y.png', isAI: true },
    { src: 'images/game/batch4/6-y.png', isAI: true },
    { src: 'images/game/batch4/7-n.png', isAI: false },
    { src: 'images/game/batch4/8-n.png', isAI: false },
    { src: 'images/game/batch4/9-y.png', isAI: true },
    { src: 'images/game/batch4/10-n.png', isAI: false },
    { src: 'images/game/batch4/11-n.png', isAI: false },
    { src: 'images/game/batch4/12-y.png', isAI: true },
    { src: 'images/game/batch4/13-y.png', isAI: true },
    { src: 'images/game/batch4/14-n.png', isAI: false },
    { src: 'images/game/batch4/15-y.png', isAI: true },
    { src: 'images/game/batch4/16-n.png', isAI: false },
    { src: 'images/game/batch4/17-n.png', isAI: false },
    { src: 'images/game/batch4/18-y.png', isAI: true },
    { src: 'images/game/batch4/19-n.png', isAI: false },
    { src: 'images/game/batch4/20-n.png', isAI: false },
    //batch 5
    { src: 'images/game/batch5/1-y.png', isAI: true },
    { src: 'images/game/batch5/2-n.png', isAI: false },
    { src: 'images/game/batch5/3-n.png', isAI: false },
    { src: 'images/game/batch5/4-y.png', isAI: true },
    { src: 'images/game/batch5/5-y.png', isAI: true },
    { src: 'images/game/batch5/6-y.png', isAI: true },
    { src: 'images/game/batch5/7-n.png', isAI: false },
    { src: 'images/game/batch5/8-n.png', isAI: false },
    { src: 'images/game/batch5/9-n.png', isAI: false },
    { src: 'images/game/batch5/10-n.png', isAI: false },
    { src: 'images/game/batch5/11-y.png', isAI: true },
    { src: 'images/game/batch5/12-y.png', isAI: true },
    { src: 'images/game/batch5/13-y.png', isAI: true },
    { src: 'images/game/batch5/14-y.png', isAI: true },
    { src: 'images/game/batch5/15-n.png', isAI: false },
    { src: 'images/game/batch5/16-n.png', isAI: false },
    { src: 'images/game/batch5/17-n.png', isAI: false },
    { src: 'images/game/batch5/18-y.png', isAI: true },
    { src: 'images/game/batch5/19-n.png', isAI: false },
    { src: 'images/game/batch5/20-y.png', isAI: true },
    // Add more images for additional batches
];

// Batches defined using slices of the images array
let batches = [
    images.slice(0, 19),  // Batch 1
    images.slice(20, 39),  // Batch 2
    images.slice(40, 59),  // Batch 3
    images.slice(60, 79),  // Batch 4
    images.slice(80, 99)  // Batch 5
];

let currentBatch = [];
let currentImageIndex = 0;
let score = 0;

// Batch Selection
document.querySelectorAll('.batch-btn').forEach(button => {
    button.addEventListener('click', () => {
        const batchNumber = parseInt(button.dataset.batch);
        selectBatch(batchNumber); // Load the selected batch
    });
});

function selectBatch(batchNumber) {
    currentBatch = batches[batchNumber - 1]; // Get the selected batch
    document.getElementById("batch-selection-screen").style.display = "none"; // Hide batch selection
    document.getElementById("intro-screen").style.display = "block"; // Show intro screen
}

// Start Game
document.getElementById("start-button").addEventListener("click", startGame);

function startGame() {
    document.getElementById("intro-screen").style.display = "none"; // Hide intro
    document.getElementById("game-screen").style.display = "block"; // Show game screen
    loadNextImage(); // Start the first image
}

// Load next image
function loadNextImage() {
    if (currentImageIndex >= currentBatch.length) {
        endGame();
        return;
    }
    const currentImage = currentBatch[currentImageIndex];
    document.getElementById("game-image").src = currentImage.src; // Update image
    document.getElementById("game-image").style.display = "block"; // Ensure image is visible
}

// Answer handling
document.getElementById("ai-btn").addEventListener("click", () => checkAnswer(true));
document.getElementById("real-btn").addEventListener("click", () => checkAnswer(false));

function checkAnswer(isAI) {
    const currentImage = currentBatch[currentImageIndex];
    if (currentImage.isAI === isAI) {
        score++; // Increase score if answer is correct
    }
    document.getElementById("score").textContent = `Score: ${score}`;
    currentImageIndex++;
    loadNextImage(); // Load next image
}

// End Game
function endGame() {
    document.getElementById("game-screen").style.display = "none"; // Hide game screen
    document.getElementById("result-screen").style.display = "block"; // Show result screen
    document.getElementById("final-score").textContent = `Your Final Score: ${score}`;
}

// Restart Game
document.getElementById("restart-button").addEventListener("click", () => {
    score = 0;
    currentImageIndex = 0;
    document.getElementById("result-screen").style.display = "none";
    document.getElementById("batch-selection-screen").style.display = "block"; // Show batch selection again
});
