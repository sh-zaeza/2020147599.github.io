let page = 1;
const booksPerPage = 9;
let selectedCategory = "";
let selectedSort = "default";

function loadBooks() {
  fetch("product.json")
    .then((response) => response.json())
    .then((data) => {
      const bookCatalog = document.getElementById("book-catalog");

      const startIndex = (page - 1) * booksPerPage;
      const endIndex = page * booksPerPage;

      const filteredBooks = selectedCategory
        ? data.filter((book) => book.genres.includes(selectedCategory))
        : data;

      const sortedBooks =
        selectedSort === "rating"
          ? filteredBooks.sort((a, b) => b.rating - a.rating)
          : filteredBooks;

      const booksToShow = sortedBooks.slice(startIndex, endIndex);

      booksToShow.forEach((book) => {
        const bookItem = document.createElement("div");
        bookItem.classList.add("book-item");
        bookItem.innerHTML = `
<img src="${book.image}" alt="">
          <h2>${book.title}</h2>
          <p>${book.author}</p>
          <p>Rating: ${book.rating}</p>
          <p class="book-price">
          ${book.title}
          <br>
          Price: $7.99 
          <br>
          Please buy the book, you'll love it!</p>
        `;
        bookCatalog.appendChild(bookItem);
      });

      if (booksToShow.length < booksPerPage) {
        window.removeEventListener("scroll", loadMoreBooks);
      }

      page++;
    })
    .catch((error) => {
      console.error("Error loading books:", error);
    });
}

function searchBooks(searchTerm) {
  const bookItems = document.getElementsByClassName("book-item");

  Array.from(bookItems).forEach((bookItem) => {
    const title = bookItem
      .getElementsByTagName("h2")[0]
      .textContent.toLowerCase();
    const author = bookItem
      .getElementsByTagName("p")[0]
      .textContent.toLowerCase();
    const description = bookItem
      .getElementsByTagName("p")[1]
      .textContent.toLowerCase();

    if (
      title.includes(searchTerm) ||
      author.includes(searchTerm) ||
      description.includes(searchTerm)
    ) {
      bookItem.style.display = "block";
    } else {
      bookItem.style.display = "none";
    }
  });
}

function filterByCategory(category) {
  selectedCategory = category;
  document.getElementById("book-catalog").innerHTML = "";
  page = 1;
  loadBooks();
}

function sortBooks(sortOption) {
  selectedSort = sortOption;
  document.getElementById("book-catalog").innerHTML = "";
  page = 1;
  loadBooks();
}

function loadMoreBooks() {
  const { scrollTop, scrollHeight, clientHeight } = document.documentElement;

  if (scrollTop + clientHeight >= scrollHeight - 5) {
    loadBooks();
  }
}

document.getElementById("search-form").addEventListener("submit", (event) => {
  event.preventDefault();
  const searchTerm = document
    .getElementById("search-input")
    .value.toLowerCase();
  searchBooks(searchTerm);
});

document
  .getElementById("category-select")
  .addEventListener("change", (event) => {
    const selectedCategory = event.target.value;
    filterByCategory(selectedCategory);
  });

document.getElementById("sort-select").addEventListener("change", (event) => {
  const selectedSort = event.target.value;
  sortBooks(selectedSort);
});

document.addEventListener("DOMContentLoaded", () => {
  loadBooks();
  window.addEventListener("scroll", loadMoreBooks);
});
