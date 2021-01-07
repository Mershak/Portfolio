
from django.contrib import admin
from django.urls import path, include
from django.conf import settings #for the media url
from django.conf.urls.static import static #for the media url
import jobs.views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('homepage/', jobs.views.home, name = 'home'),
    path('blog/', include('blog.urls')),

] + static(settings.MEDIA_URL,document_root = settings.MEDIA_ROOT)  #connects the media image path to a url
