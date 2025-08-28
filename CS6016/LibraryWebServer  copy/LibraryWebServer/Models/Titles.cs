using System;
using System.Collections.Generic;

namespace LibraryWebServer.Models;

public partial class Titles
{
    public string Isbn { get; set; } = null!;

    public string Title { get; set; } = null!;

    public string Author { get; set; } = null!;
    //one title can have many physical copies, we got this from the fks
    

    public virtual ICollection<Inventory> Inventory { get; set; } = new List<Inventory>();
}
