from django.shortcuts import render,redirect
from django import forms
from django.http import HttpResponseRedirect
from django.urls import reverse

from markdown2 import markdown

from . import util

class wikiForm(forms.Form):
    entryTitle = forms.CharField(label="Article Title",
                                 max_length=24,
                                 widget=forms.TextInput(attrs={'class': 'form-control bg-dark text-light'}))
    mdContent = forms.CharField(widget=forms.Textarea(attrs={
        'class': 'form-control',
        'background-color':'gray'
        })
    )
    

def index(request):
    return render(request, "encyclopedia/index.html", {
        "entries": util.list_entries()
    })

def createEntry(request):
    if request.method == "POST":
        form = wikiForm(request.POST)
        if form.is_valid():
            util.save_entry(form.cleaned_data["entryTitle"],form.cleaned_data["mdContent"])
        return HttpResponseRedirect(reverse("wiki:index"))
    else:
        form = wikiForm()
        return render(request,"encyclopedia/create.html",{
            "form":form
        })

def wikiEntry(request,title):
    if title == "None":
        title,entry = util.random_entry()
    else:
        entry = util.get_entry(title)

    return render(request,"encyclopedia/entry.html",{
        "title":title,
        "entry":markdown(entry)
    })

def mdEntry(request, title):
    if request.method == "POST":
        form = wikiForm(request.POST)
        if form.is_valid():
            util.save_entry(form.cleaned_data["entryTitle"], form.cleaned_data["mdContent"])
            return HttpResponseRedirect(reverse("wiki:index"))
    else:
        form = wikiForm(initial={
            "entryTitle": title,
            "mdContent": util.get_entry(title)
        })
    
    return render(request, "encyclopedia/create.html", {
        "form": form
    })

def search(request):

    query = request.GET.get("q", "").strip()

    if util.get_entry(query):
        # Exact match — redirect to entry
        return redirect("wiki:entry", title=query)

    # Partial match (case-insensitive)
    entries = util.list_entries()
    results = [entry for entry in entries if query.lower() in entry.lower()]

    return render(request, "encyclopedia/search.html", {
        "query": query,
        "results": results
    })
