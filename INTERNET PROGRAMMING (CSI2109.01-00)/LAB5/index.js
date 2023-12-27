const express = require("express");
const path = require("path");
const fs = require("fs");
const sqlite3 = require("sqlite3").verbose();
const bodyParser = require("body-parser");

const app = express();
const port = 3000;

// Create a new database connection
const db = new sqlite3.Database("product.db");

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Serve static files from the "public" directory
app.use(express.static("public"));

// Serve the index.html file as the root route
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "index.html"));
});

// Retrieve a book by its ID
app.get("/product/:product_id", (req, res) => {
  const bookId = req.params.product_id;

  // Read the corresponding HTML file based on the book ID
  const fileName = `${bookId}.html`;
  const filePath = path.join(__dirname, "public", "product", fileName);

  fs.readFile(filePath, "utf8", (err, data) => {
    if (err) {
      console.error(err);
      res.status(404).send("File not found");
    } else {
      res.send(data);
    }
  });
});

// Retrieve a book by its ID
app.get("/products/:product_id", (req, res) => {
  const bookId = req.params.product_id;

  const query = `SELECT * FROM product WHERE product_id = ?`;

  db.get(query, [bookId], (err, row) => {
    if (err) {
      console.error(err.message);
      res.status(500).send("Internal Server Error");
      return;
    }

    if (!row) {
      res.status(404).send("Book not found");
      return;
    }
    res.json(row);
  });
});

// Retrieve all books
app.get("/product", (req, res) => {
  const query = `SELECT * FROM product`;

  db.all(query, [], (err, rows) => {
    if (err) {
      console.error(err.message);
      res.status(500).send("Internal Server Error");
      return;
    }

    res.json(rows);
  });
});

// Handle GET request to fetch comments for a specific book
app.get("/comment/:product_id", (req, res) => {
  const { product_id } = req.params;

  // Read comment data from the comment.json file
  fs.readFile("comment.json", "utf8", (err, data) => {
    if (err) {
      console.error(err);
      res.status(500).send("Internal Server Error");
      return;
    }

    try {
      const comments = JSON.parse(data);

      // Find the comments for the specific bookId
      const bookComments = comments.find(
        (comment) => comment.product_id === Number(product_id)
      );

      if (bookComments) {
        res.json({ comments: bookComments.comments });
      } else {
        res.json({ comments: [] });
      }
    } catch (error) {
      console.error(error);
      res.status(500).send("Internal Server Error");
    }
  });
});

app.get("/comment", (req, res) => {
  fs.readFile("comment.json", "utf8", (err, data) => {
    if (err) {
      console.error(err);
      res.status(500).json({ error: "Failed to read names." });
      return;
    }
    res.json(JSON.parse(data));
  });
});

// Route to add a comment to a book by ID
// Route to add a comment to a book by ID
app.post("/comment/:product_id", (req, res) => {
  const bookId = req.params.product_id;
  const comment = req.body.comment;

  // Read the comment.json file
  fs.readFile("comment.json", "utf8", (err, data) => {
    if (err) {
      console.error(err);
      res.status(500).send("Internal Server Error");
      return;
    }

    const commentsData = JSON.parse(data);
    const bookComments = commentsData.find(
      (item) => item.product_id === bookId
    );

    if (bookComments) {
      // Add the new comment to the comments array
      bookComments.comments.push(comment);
    }

    // Write the updated comments data back to the comment.json file
    fs.writeFile("comment.json", JSON.stringify(commentsData), (err) => {
      if (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
        return;
      }

      console.log("Updated Comments Data:", commentsData);
      res.json({ message: "Comment added successfully." });
    });
  });
});

app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});
