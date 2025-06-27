from django.urls import path

from . import views

app_name = "wiki"

urlpatterns = [
    path("", views.index, name="index"),
    path("create",views.createEntry,name="create"),
    path("search",views.search,name="search"),
    path("edit/<str:title>",views.mdEntry,name="edit"),
    path("<str:title>",views.wikiEntry,name="entry")
]
